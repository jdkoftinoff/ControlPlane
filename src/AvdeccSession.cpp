#include "ControlPlane/World.hpp"
#include "ControlPlane/AvdeccSession.hpp"

namespace ControlPlane
{

AvdeccSession::AvdeccSession( SchemaAvdeccAdaptor &schema,
                              ControlIdentityComparatorPtr write_access,
                              uint32_t &available_index,
                              function<void(const FixedBuffer &)> sender,
                              Eui64 local_entity_id,
                              Eui64 controller_entity_id )
    : m_schema( schema )
    , m_notifier( schema )
    , m_write_access( write_access )
    , m_available_index( available_index )
    , m_sender( sender )
    , m_local_entity_id( local_entity_id )
    , m_controller_entity_id( controller_entity_id )
    , m_last_tick_time_in_milliseconds( Milliseconds( 0 ) )
{
}

void AvdeccSession::tick( Milliseconds current_time_in_milliseconds ) {}

void AvdeccSession::sendReply( const FixedBuffer &request, uint8_t response_status )
{
    if ( m_sender )
    {
        FixedBufferWithSize<1500> response;
        response.putBuf( request );

        {
            uint8_t control_data = response.getOctet( 1 );
            control_data |= 1; /* change command to response */
            response.setOctet( control_data, 1 );
        }

        {
            uint8_t status = response.getOctet( 2 );
            status &= 0x7;
            status |= ( response_status << 3 );
            response.setOctet( status, 2 );
        }

        {
            uint16_t control_data_length = response.getDoublet( 2 );
            control_data_length &= 0xf800;
            control_data_length |= response.getLength() - 12;
        }

        m_sender( response );
    }
}

void AvdeccSession::sendResponse( const FixedBuffer &response, Eui64 originating_controller_entity_id )
{
    if ( m_sender )
    {
        FixedBufferWithSize<1500> updated_response;
        updated_response.putBuf( response );
        updated_response.setEUI64( m_controller_entity_id, AVDECC_AECPDU_AEM_OFFSET_CONTROLLER_ENTITY_ID );

        bool unsolicited = false;

        if ( originating_controller_entity_id != m_controller_entity_id )
        {
            unsolicited = true;
        }

        {
            uint16_t command_type = updated_response.getDoublet( AVDECC_AECPDU_AEM_OFFSET_COMMAND_TYPE );
            if ( unsolicited )
            {
                command_type |= 0x8000;
            }
            else
            {
                command_type &= 0x7fff;
            }
            updated_response.setDoublet( command_type, AVDECC_AECPDU_AEM_OFFSET_COMMAND_TYPE );
        }

        m_sender( updated_response );
    }
}

void AvdeccSession::receivedPdu( const FixedBuffer &pdu )
{
    bool handled = false;

    // we already know the message is AVTP ethertype and is either directly
    // targetting my MAC address or is a multicast message

    // Try see if it is an AEM message
    uint8_t subtype = pdu.getOctet( AVDECC_COMMON_CONTROL_HEADER_SUBTYPE_OFFSET );

    uint8_t version = ( pdu.getOctet( AVDECC_COMMON_CONTROL_HEADER_VERSION_OFFSET )
                        & AVDECC_COMMON_CONTROL_HEADER_VERSION_MASK ) >> AVDECC_COMMON_CONTROL_HEADER_VERSION_SHIFT;

    uint8_t message_type = ( pdu.getOctet( AVDECC_COMMON_CONTROL_HEADER_SV_VERSION_CONTROL_DATA_OFFSET )
                             & AVDECC_COMMON_CONTROL_HEADER_CONTROL_DATA_MASK )
                           >> AVDECC_COMMON_CONTROL_HEADER_CONTROL_DATA_SHIFT;

    uint8_t status_code = ( pdu.getOctet( AVDECC_COMMON_CONTROL_HEADER_CONTROL_STATUS_OFFSET )
                            & AVDECC_COMMON_CONTROL_HEADER_CONTROL_STATUS_MASK )
                          >> AVDECC_COMMON_CONTROL_HEADER_CONTROL_STATUS_SHIFT;

    uint16_t control_data_length = ( pdu.getDoublet( AVDECC_COMMON_CONTROL_HEADER_CONTROL_DATA_LENGTH_OFFSET )
                                     & AVDECC_COMMON_CONTROL_HEADER_CONTROL_DATA_LENGTH_MASK )
                                   >> AVDECC_COMMON_CONTROL_HEADER_CONTROL_DATA_LENGTH_SHIFT;

    if ( version == 0 && subtype == AVDECC_SUBTYPE_AECP )
    {
        Eui64 target_entity_id = pdu.getEUI64( AVDECC_COMMON_CONTROL_HEADER_OFFSET_STREAM_ID );
        Eui64 controller_entity_id = pdu.getEUI64( AVDECC_AECPDU_AEM_OFFSET_CONTROLLER_ENTITY_ID );

        if ( message_type == AVDECC_AECP_MESSAGE_TYPE_AEM_COMMAND )
        {
            if ( target_entity_id == m_local_entity_id )
            {
                if ( controller_entity_id == m_controller_entity_id )
                {
                    // handle it here only if the AEM Command is for me and is from the controller that
                    // I am tied to
                    receivedAemCommand( pdu );
                }
                handled = true;
            }
        }
        else if ( message_type == AVDECC_AECP_MESSAGE_TYPE_AEM_RESPONSE )
        {
            if ( controller_entity_id == m_local_entity_id )
            {
                if ( target_entity_id == m_controller_entity_id )
                {
                    // handle it here only if the AEM Response for me and is from the entity that
                    // I am tied to
                    receivedAemResponse( pdu );
                }
                handled = true;
            }
        }
        else if ( message_type == AVDECC_AECP_MESSAGE_TYPE_ADDRESS_ACCESS_COMMAND )
        {
            if ( target_entity_id == m_local_entity_id )
            {
                if ( controller_entity_id == m_controller_entity_id )
                {
                    // handle it here only if the AA Command is for me and is from the controller that
                    // I am tied to
                    receivedAACommand( pdu );
                }
                handled = true;
            }
        }
        else if ( message_type == AVDECC_AECP_MESSAGE_TYPE_ADDRESS_ACCESS_RESPONSE )
        {
            if ( controller_entity_id == m_local_entity_id )
            {
                if ( target_entity_id == m_controller_entity_id )
                {
                    // handle it here only if the AA Response for me and is from the entity that
                    // I am tied to
                    receivedAAResponse( pdu );
                }
                handled = true;
            }
        }
    }
}

void AvdeccSession::receivedAemCommand( const FixedBuffer &pdu )
{
    uint16_t sequence_id = pdu.getDoublet( AVDECC_AECPDU_AEM_OFFSET_SEQUENCE_ID );

    // only deal with messages with sequence_id's that we haven't seen yet
    if ( m_received_aem_command_sequence_ids.find( sequence_id ) == m_received_aem_command_sequence_ids.end() )
    {
        uint16_t command_type = pdu.getDoublet( AVDECC_AECPDU_AEM_OFFSET_COMMAND_TYPE ) & 0x7fff;

        switch ( command_type )
        {
        case AVDECC_AEM_COMMAND_ACQUIRE_ENTITY:
            acquireEntity( pdu );
            break;
        case AVDECC_AEM_COMMAND_LOCK_ENTITY:
            lockEntity( pdu );
            break;
        case AVDECC_AEM_COMMAND_ENTITY_AVAILABLE:
            entityAvailable( pdu );
            break;
        case AVDECC_AEM_COMMAND_CONTROLLER_AVAILABLE:
            controllerAvailable( pdu );
            break;
        case AVDECC_AEM_COMMAND_READ_DESCRIPTOR:
            readDescriptor( pdu );
            break;
        case AVDECC_AEM_COMMAND_WRITE_DESCRIPTOR:
            writeDescriptor( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_CONFIGURATION:
            setConfiguration( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_CONFIGURATION:
            getConfiguration( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_STREAM_FORMAT:
            setStreamFormat( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_STREAM_FORMAT:
            getStreamFormat( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_VIDEO_FORMAT:
            setVideoFormat( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_VIDEO_FORMAT:
            getVideoFormat( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_SENSOR_FORMAT:
            setSensorFormat( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_SENSOR_FORMAT:
            getSensorFormat( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_STREAM_INFO:
            setStreamInfo( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_STREAM_INFO:
            getStreamInfo( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_NAME:
            setName( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_NAME:
            getName( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_ASSOCIATION_ID:
            setAssociationId( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_ASSOCIATION_ID:
            getAssociationId( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_SAMPLING_RATE:
            setSamplingRate( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_SAMPLING_RATE:
            getSamplingRate( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_CLOCK_SOURCE:
            setClockSource( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_CLOCK_SOURCE:
            getClockSource( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_CONTROL:
            setControl( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_CONTROL:
            getControl( pdu );
            break;
        case AVDECC_AEM_COMMAND_INCREMENT_CONTROL:
            incrementControl( pdu );
            break;
        case AVDECC_AEM_COMMAND_DECREMENT_CONTROL:
            decrementControl( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_SIGNAL_SELECTOR:
            setSignalSelector( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_SIGNAL_SELECTOR:
            getSignalSelector( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_MIXER:
            setMixer( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_MIXER:
            getMixer( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_MATRIX:
            setMatrix( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_MATRIX:
            getMatrix( pdu );
            break;
        case AVDECC_AEM_COMMAND_START_STREAMING:
            startStreaming( pdu );
            break;
        case AVDECC_AEM_COMMAND_STOP_STREAMING:
            stopStreaming( pdu );
            break;
        case AVDECC_AEM_COMMAND_REGISTER_UNSOLICITED_NOTIFICATION:
            registerUnsolicitedNotifications( pdu );
            break;
        case AVDECC_AEM_COMMAND_DEREGISTER_UNSOLICITED_NOTIFICATION:
            deregisterUnsolicitedNotifications( pdu );
            break;
        case AVDECC_AEM_COMMAND_IDENTIFY_NOTIFICATION:
            identifyNotification( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_AVB_INFO:
            getAvbInfo( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_AS_PATH:
            getAsPath( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_COUNTERS:
            getCounters( pdu );
            break;
        case AVDECC_AEM_COMMAND_REBOOT:
            reboot( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_AUDIO_MAP:
            getAudioMap( pdu );
            break;
        case AVDECC_AEM_COMMAND_ADD_AUDIO_MAPPINGS:
            addAudioMappings( pdu );
            break;
        case AVDECC_AEM_COMMAND_REMOVE_AUDIO_MAPPINGS:
            removeAudioMappings( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_VIDEO_MAP:
            getVideoMap( pdu );
            break;
        case AVDECC_AEM_COMMAND_ADD_VIDEO_MAPPINGS:
            addVideoMappings( pdu );
            break;
        case AVDECC_AEM_COMMAND_REMOVE_VIDEO_MAPPINGS:
            removeVideoMappings( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_SENSOR_MAP:
            getSensorMap( pdu );
            break;
        case AVDECC_AEM_COMMAND_ADD_SENSOR_MAPPINGS:
            addSensorMappings( pdu );
            break;
        case AVDECC_AEM_COMMAND_REMOVE_SENSOR_MAPPINGS:
            removeSensorMappings( pdu );
            break;
        case AVDECC_AEM_COMMAND_START_OPERATION:
            startOperation( pdu );
            break;
        case AVDECC_AEM_COMMAND_ABORT_OPERATION:
            abortOperation( pdu );
            break;
        case AVDECC_AEM_COMMAND_OPERATION_STATUS:
            operationStatus( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_ADD_KEY:
            authAddKey( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_DELETE_KEY:
            authDeleteKey( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_GET_KEY_LIST:
            authGetKeyList( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_GET_KEY:
            authGetKey( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_ADD_KEY_TO_CHAIN:
            authAddKeyToChain( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_DELETE_KEY_FROM_CHAIN:
            authDeleteKeyFromChain( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_GET_KEYCHAIN_LIST:
            authGetKeychainList( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_GET_IDENTITY:
            authGetIdentity( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_ADD_TOKEN:
            authAddToken( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_DELETE_TOKEN:
            authDeleteToken( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTHENTICATE:
            authenticate( pdu );
            break;
        case AVDECC_AEM_COMMAND_DEAUTHENTICATE:
            deauthenticate( pdu );
            break;
        case AVDECC_AEM_COMMAND_ENABLE_TRANSPORT_SECURITY:
            enableTransportSecurity( pdu );
            break;
        case AVDECC_AEM_COMMAND_DISABLE_TRANSPORT_SECURITY:
            disableTransportSecurity( pdu );
            break;
        case AVDECC_AEM_COMMAND_ENABLE_STREAM_ENCRYPTION:
            enableStreamEncryption( pdu );
            break;
        case AVDECC_AEM_COMMAND_DISABLE_STREAM_ENCRYPTION:
            disableStreamEncryption( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_MEMORY_OBJECT_LENGTH:
            setMemoryObjectLength( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_MEMORY_OBJECT_LENGTH:
            getMemoryObjectLength( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_STREAM_BACKUP:
            setStreamBackup( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_STREAM_BACKUP:
            getStreamBackup( pdu );
            break;
        default:
            sendReply( pdu, AVDECC_AECP_STATUS_NOT_IMPLEMENTED );
            break;
        }

        // clear out any old sequence_ids from the history
        for ( auto i = m_received_aem_command_sequence_ids.begin(); i != m_received_aem_command_sequence_ids.end(); )
        {
            uint16_t diff = sequence_id - *i;

            if ( diff > 512 )
            {
                m_received_aem_command_sequence_ids.erase( i++ );
            }
            else
            {
                ++i;
            }
        }
    }
}

void AvdeccSession::receivedAemResponse( const FixedBuffer &pdu )
{
    uint16_t sequence_id = pdu.getDoublet( AVDECC_AECPDU_AEM_OFFSET_SEQUENCE_ID );

    // only deal with messages with sequence_id's that we haven't seen yet
    if ( m_received_aem_response_sequence_ids.find( sequence_id ) == m_received_aem_response_sequence_ids.end() )
    {
        uint16_t command_type = pdu.getDoublet( AVDECC_AECPDU_AEM_OFFSET_COMMAND_TYPE ) & 0x7fff;

        switch ( command_type )
        {
        case AVDECC_AEM_COMMAND_ACQUIRE_ENTITY:
            acquireEntityResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_LOCK_ENTITY:
            lockEntityResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_ENTITY_AVAILABLE:
            entityAvailableResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_CONTROLLER_AVAILABLE:
            controllerAvailableResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_READ_DESCRIPTOR:
            readDescriptorResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_WRITE_DESCRIPTOR:
            writeDescriptorResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_CONFIGURATION:
            setConfigurationResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_CONFIGURATION:
            getConfigurationResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_STREAM_FORMAT:
            setStreamFormatResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_STREAM_FORMAT:
            getStreamFormatResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_VIDEO_FORMAT:
            setVideoFormatResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_VIDEO_FORMAT:
            getVideoFormatResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_SENSOR_FORMAT:
            setSensorFormatResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_SENSOR_FORMAT:
            getSensorFormatResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_STREAM_INFO:
            setStreamInfoResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_STREAM_INFO:
            getStreamInfoResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_NAME:
            setNameResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_NAME:
            getNameResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_ASSOCIATION_ID:
            setAssociationIdResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_ASSOCIATION_ID:
            getAssociationIdResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_SAMPLING_RATE:
            setSamplingRateResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_SAMPLING_RATE:
            getSamplingRateResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_CLOCK_SOURCE:
            setClockSourceResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_CLOCK_SOURCE:
            getClockSourceResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_CONTROL:
            setControlResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_CONTROL:
            getControlResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_INCREMENT_CONTROL:
            incrementControlResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_DECREMENT_CONTROL:
            decrementControlResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_SIGNAL_SELECTOR:
            setSignalSelectorResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_SIGNAL_SELECTOR:
            getSignalSelectorResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_MIXER:
            setMixerResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_MIXER:
            getMixerResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_MATRIX:
            setMatrixResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_MATRIX:
            getMatrixResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_START_STREAMING:
            startStreamingResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_STOP_STREAMING:
            stopStreamingResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_REGISTER_UNSOLICITED_NOTIFICATION:
            registerUnsolicitedNotificationsResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_DEREGISTER_UNSOLICITED_NOTIFICATION:
            deregisterUnsolicitedNotificationsResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_IDENTIFY_NOTIFICATION:
            identifyNotificationResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_AVB_INFO:
            getAvbInfoResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_AS_PATH:
            getAsPathResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_COUNTERS:
            getCountersResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_REBOOT:
            rebootResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_AUDIO_MAP:
            getAudioMapResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_ADD_AUDIO_MAPPINGS:
            addAudioMappingsResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_REMOVE_AUDIO_MAPPINGS:
            removeAudioMappingsResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_VIDEO_MAP:
            getVideoMapResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_ADD_VIDEO_MAPPINGS:
            addVideoMappingsResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_REMOVE_VIDEO_MAPPINGS:
            removeVideoMappingsResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_SENSOR_MAP:
            getSensorMapResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_ADD_SENSOR_MAPPINGS:
            addSensorMappingsResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_REMOVE_SENSOR_MAPPINGS:
            removeSensorMappingsResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_START_OPERATION:
            startOperationResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_ABORT_OPERATION:
            abortOperationResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_OPERATION_STATUS:
            operationStatusResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_ADD_KEY:
            authAddKeyResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_DELETE_KEY:
            authDeleteKeyResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_GET_KEY_LIST:
            authGetKeyListResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_GET_KEY:
            authGetKeyResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_ADD_KEY_TO_CHAIN:
            authAddKeyToChainResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_DELETE_KEY_FROM_CHAIN:
            authDeleteKeyFromChainResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_GET_KEYCHAIN_LIST:
            authGetKeychainListResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_GET_IDENTITY:
            authGetIdentityResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_ADD_TOKEN:
            authAddTokenResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTH_DELETE_TOKEN:
            authDeleteTokenResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_AUTHENTICATE:
            authenticateResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_DEAUTHENTICATE:
            deauthenticateResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_ENABLE_TRANSPORT_SECURITY:
            enableTransportSecurityResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_DISABLE_TRANSPORT_SECURITY:
            disableTransportSecurityResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_ENABLE_STREAM_ENCRYPTION:
            enableStreamEncryptionResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_DISABLE_STREAM_ENCRYPTION:
            disableStreamEncryptionResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_MEMORY_OBJECT_LENGTH:
            setMemoryObjectLengthResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_MEMORY_OBJECT_LENGTH:
            getMemoryObjectLengthResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_SET_STREAM_BACKUP:
            setStreamBackupResponse( pdu );
            break;
        case AVDECC_AEM_COMMAND_GET_STREAM_BACKUP:
            getStreamBackupResponse( pdu );
            break;
        default:
            break;
        }

        // clear out any old sequence_ids from the history
        for ( auto i = m_received_aem_response_sequence_ids.begin(); i != m_received_aem_response_sequence_ids.end(); )
        {
            uint16_t diff = sequence_id - *i;

            if ( diff > 512 )
            {
                m_received_aem_response_sequence_ids.erase( i++ );
            }
            else
            {
                ++i;
            }
        }
    }
}

void AvdeccSession::receivedAACommand( const FixedBuffer &request )
{
    FixedBufferWithSize<1500> pdu;
    pdu.putBuf( request );

    uint16_t sequence_id = pdu.getDoublet( AVDECC_AECPDU_AA_OFFSET_SEQUENCE_ID );

    if ( m_received_aa_command_sequence_ids.find( sequence_id ) == m_received_aa_command_sequence_ids.end() )
    {
        uint16_t tlv_count = pdu.getDoublet( AVDECC_AECPDU_AA_OFFSET_TLV_COUNT );

        // go through the TLV's and dispatch the read/writes and respond
        uint8_t *p = pdu.getBuf() + AVDECC_AECPDU_AA_LEN;
        uint8_t aa_status = AVDECC_AECPDU_AA_STATUS_NOT_IMPLEMENTED;

        for ( uint16_t i = 0; i < tlv_count; ++i )
        {
            // See 9.2.1.3.3
            uint8_t tlv_mode = ( p[AVDECC_AECPDU_AA_TLV_OFFSET_MODE_LENGTH] >> 4 ) & 0xf;

            uint16_t tlv_length = ( ( ( uint16_t )( p[AVDECC_AECPDU_AA_TLV_OFFSET_MODE_LENGTH] & 0xf ) ) << 4 )
                                  + p[AVDECC_AECPDU_AA_TLV_OFFSET_MODE_LENGTH + 1];

            uint64_t tlv_address = ( (uint64_t)p[0] << ( 7 * 8 ) ) + ( (uint64_t)p[0] << ( 6 * 8 ) )
                                   + ( (uint64_t)p[0] << ( 5 * 8 ) ) + ( (uint64_t)p[0] << ( 4 * 8 ) )
                                   + ( (uint64_t)p[0] << ( 3 * 8 ) ) + ( (uint64_t)p[0] << ( 2 * 8 ) )
                                   + ( (uint64_t)p[0] << ( 1 * 8 ) ) + ( (uint64_t)p[0] << ( 0 * 8 ) );

            switch ( tlv_mode )
            {
            case AVDECC_AECPDU_AA_MODE_READ:
            {
                FixedBuffer data( p + AVDECC_AECPDU_AA_TLV_LEN, tlv_length );
                data.setLength( tlv_length );
                aa_status = receivedAARead( pdu, tlv_address, data );
                break;
            }
            case AVDECC_AECPDU_AA_MODE_WRITE:
            {
                FixedBuffer data( p + AVDECC_AECPDU_AA_TLV_LEN, tlv_length );
                data.setLength( tlv_length );
                aa_status = receivedAAWrite( pdu, tlv_address, data );
                break;
            }
            case AVDECC_AECPDU_AA_MODE_EXECUTE:
            {
                aa_status = receivedAAExecute( pdu, tlv_address );
                break;
            }
            }

            p = p + AVDECC_AECPDU_AA_TLV_LEN + tlv_length;
            if ( aa_status != AVDECC_AECPDU_AA_STATUS_SUCCESS )
            {
                break;
            }
        }
        // Send the response to either just the requesting controller or it and all
        // registered controllers
        pdu.setOctet( ( pdu.getOctet( 2 ) & 0x7 ) + ( aa_status << 3 ), 2 );

        sendReply( pdu, aa_status );

        // clear out any old sequence_ids from the history
        for ( auto i = m_received_aa_command_sequence_ids.begin(); i != m_received_aa_command_sequence_ids.end(); )
        {
            uint16_t diff = sequence_id - *i;

            if ( diff > 512 )
            {
                m_received_aa_command_sequence_ids.erase( i++ );
            }
            else
            {
                ++i;
            }
        }
    }
}

void AvdeccSession::receivedAAResponse( const FixedBuffer &request ) {}

uint8_t AvdeccSession::receivedAAWrite( const FixedBuffer &request, uint64_t address, const FixedBuffer &data )
{
    return AVDECC_AECPDU_AA_STATUS_NOT_IMPLEMENTED;
}

uint8_t AvdeccSession::receivedAARead( const FixedBuffer &request, uint64_t address, FixedBuffer &data )
{
    return AVDECC_AECPDU_AA_STATUS_NOT_IMPLEMENTED;
}

uint8_t AvdeccSession::receivedAAExecute( const FixedBuffer &request, uint64_t address )
{
    return AVDECC_AECPDU_AA_STATUS_NOT_IMPLEMENTED;
}
}
