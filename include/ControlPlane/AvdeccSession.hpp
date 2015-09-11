#pragma once

#include "World.hpp"
#include "Avdecc.hpp"
#include "AvdeccSchema.hpp"

namespace ControlPlane
{

struct AvdeccSession
{
    SchemaAvdeccAdaptor &m_schema;
    SchemaAvdeccAdaptor::ChangeNotifierHolder m_notifier;
    ControlIdentityComparatorPtr m_write_access;
    uint32_t &m_available_index;
    function<void(FixedBuffer const &)> m_sender;
    Eui64 m_local_entity_id;
    Eui64 m_controller_entity_id;
    Milliseconds m_last_tick_time_in_milliseconds;
    std::set<uint16_t> m_received_aem_command_sequence_ids;
    std::set<uint16_t> m_received_aem_response_sequence_ids;

  public:
    AvdeccSession( SchemaAvdeccAdaptor &schema,
                   ControlIdentityComparatorPtr write_access,
                   uint32_t &available_index,
                   function<void(FixedBuffer const &)> sender,
                   Eui64 local_entity_id,
                   Eui64 controller_entity_id );

    virtual ~AvdeccSession() {}

    virtual void tick( Milliseconds current_time_in_milliseconds );

    virtual void sendReply( FixedBuffer const &request, uint8_t response_status );

    virtual void sendResponse( FixedBuffer const &response, Eui64 originating_controller_entity_id );

    virtual void receivedPdu( FixedBuffer const &pdu );

    virtual void receivedAemCommand( FixedBuffer const &request );

    virtual void receivedAemResponse( FixedBuffer const &request );

    virtual void receivedAACommand( FixedBuffer const &request );

    virtual void receivedAAResponse( FixedBuffer const &request );

    virtual void receivedAAWrite( FixedBuffer const &request, uint64_t address, FixedBuffer const &data );

    virtual void receivedAARead( FixedBuffer const &request, uint64_t address, FixedBuffer &data );

    virtual void receivedAAExecute( FixedBuffer const &request, uint64_t address );

    virtual void acquireEntity( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void lockEntity( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void entityAvailable( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void controllerAvailable( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void readDescriptor( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void writeDescriptor( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void setConfiguration( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getConfiguration( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void setStreamFormat( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getStreamFormat( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void setVideoFormat( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getVideoFormat( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void setSensorFormat( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getSensorFormat( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void setStreamInfo( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getStreamInfo( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void setName( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getName( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void setAssociationId( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getAssociationId( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void setSamplingRate( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getSamplingRate( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void setClockSource( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getClockSource( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void setControl( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getControl( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void incrementControl( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void decrementControl( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void setSignalSelector( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getSignalSelector( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void setMixer( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getMixer( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void setMatrix( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getMatrix( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void startStreaming( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void stopStreaming( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void registerUnsolicitedNotifications( FixedBuffer const &request )
    {
        sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED );
    }

    virtual void deregisterUnsolicitedNotifications( FixedBuffer const &request )
    {
        sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED );
    }

    virtual void identifyNotification( FixedBuffer const &request )
    {
        sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED );
    }

    virtual void getAvbInfo( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getAsPath( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getCounters( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void reboot( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getAudioMap( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void addAudioMappings( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void removeAudioMappings( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getVideoMap( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void addVideoMappings( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void removeVideoMappings( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getSensorMap( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void addSensorMappings( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void removeSensorMappings( FixedBuffer const &request )
    {
        sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED );
    }

    virtual void startOperation( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void abortOperation( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void operationStatus( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void authAddKey( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void authDeleteKey( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void authGetKeyList( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void authGetKey( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void authAddKeyToChain( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void authDeleteKeyFromChain( FixedBuffer const &request )
    {
        sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED );
    }

    virtual void authGetKeychainList( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void authGetIdentity( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void authAddToken( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void authDeleteToken( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void authenticate( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void deauthenticate( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void enableTransportSecurity( FixedBuffer const &request )
    {
        sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED );
    }

    virtual void disableTransportSecurity( FixedBuffer const &request )
    {
        sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED );
    }

    virtual void enableStreamEncryption( FixedBuffer const &request )
    {
        sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED );
    }

    virtual void disableStreamEncryption( FixedBuffer const &request )
    {
        sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED );
    }

    virtual void setMemoryObjectLength( FixedBuffer const &request )
    {
        sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED );
    }

    virtual void getMemoryObjectLength( FixedBuffer const &request )
    {
        sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED );
    }

    virtual void setStreamBackup( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void getStreamBackup( FixedBuffer const &request ) { sendReply( request, AVDECC_AECP_STATUS_NOT_IMPLEMENTED ); }

    virtual void acquireEntityResponse( FixedBuffer const &request ) {}

    virtual void lockEntityResponse( FixedBuffer const &request ) {}

    virtual void entityAvailableResponse( FixedBuffer const &request ) {}

    virtual void controllerAvailableResponse( FixedBuffer const &request ) {}

    virtual void readDescriptorResponse( FixedBuffer const &request ) {}

    virtual void writeDescriptorResponse( FixedBuffer const &request ) {}

    virtual void setConfigurationResponse( FixedBuffer const &request ) {}

    virtual void getConfigurationResponse( FixedBuffer const &request ) {}

    virtual void setStreamFormatResponse( FixedBuffer const &request ) {}

    virtual void getStreamFormatResponse( FixedBuffer const &request ) {}

    virtual void setVideoFormatResponse( FixedBuffer const &request ) {}

    virtual void getVideoFormatResponse( FixedBuffer const &request ) {}

    virtual void setSensorFormatResponse( FixedBuffer const &request ) {}

    virtual void getSensorFormatResponse( FixedBuffer const &request ) {}

    virtual void setStreamInfoResponse( FixedBuffer const &request ) {}

    virtual void getStreamInfoResponse( FixedBuffer const &request ) {}

    virtual void setNameResponse( FixedBuffer const &request ) {}

    virtual void getNameResponse( FixedBuffer const &request ) {}

    virtual void setAssociationIdResponse( FixedBuffer const &request ) {}

    virtual void getAssociationIdResponse( FixedBuffer const &request ) {}

    virtual void setSamplingRateResponse( FixedBuffer const &request ) {}

    virtual void getSamplingRateResponse( FixedBuffer const &request ) {}

    virtual void setClockSourceResponse( FixedBuffer const &request ) {}

    virtual void getClockSourceResponse( FixedBuffer const &request ) {}

    virtual void setControlResponse( FixedBuffer const &request ) {}

    virtual void getControlResponse( FixedBuffer const &request ) {}

    virtual void incrementControlResponse( FixedBuffer const &request ) {}

    virtual void decrementControlResponse( FixedBuffer const &request ) {}

    virtual void setSignalSelectorResponse( FixedBuffer const &request ) {}

    virtual void getSignalSelectorResponse( FixedBuffer const &request ) {}

    virtual void setMixerResponse( FixedBuffer const &request ) {}

    virtual void getMixerResponse( FixedBuffer const &request ) {}

    virtual void setMatrixResponse( FixedBuffer const &request ) {}

    virtual void getMatrixResponse( FixedBuffer const &request ) {}

    virtual void startStreamingResponse( FixedBuffer const &request ) {}

    virtual void stopStreamingResponse( FixedBuffer const &request ) {}

    virtual void registerUnsolicitedNotificationsResponse( FixedBuffer const &request ) {}

    virtual void deregisterUnsolicitedNotificationsResponse( FixedBuffer const &request ) {}

    virtual void identifyNotificationResponse( FixedBuffer const &request ) {}

    virtual void getAvbInfoResponse( FixedBuffer const &request ) {}

    virtual void getAsPathResponse( FixedBuffer const &request ) {}

    virtual void getCountersResponse( FixedBuffer const &request ) {}

    virtual void rebootResponse( FixedBuffer const &request ) {}

    virtual void getAudioMapResponse( FixedBuffer const &request ) {}

    virtual void addAudioMappingsResponse( FixedBuffer const &request ) {}

    virtual void removeAudioMappingsResponse( FixedBuffer const &request ) {}

    virtual void getVideoMapResponse( FixedBuffer const &request ) {}

    virtual void addVideoMappingsResponse( FixedBuffer const &request ) {}

    virtual void removeVideoMappingsResponse( FixedBuffer const &request ) {}

    virtual void getSensorMapResponse( FixedBuffer const &request ) {}

    virtual void addSensorMappingsResponse( FixedBuffer const &request ) {}

    virtual void removeSensorMappingsResponse( FixedBuffer const &request ) {}

    virtual void startOperationResponse( FixedBuffer const &request ) {}

    virtual void abortOperationResponse( FixedBuffer const &request ) {}

    virtual void operationStatusResponse( FixedBuffer const &request ) {}

    virtual void authAddKeyResponse( FixedBuffer const &request ) {}

    virtual void authDeleteKeyResponse( FixedBuffer const &request ) {}

    virtual void authGetKeyListResponse( FixedBuffer const &request ) {}

    virtual void authGetKeyResponse( FixedBuffer const &request ) {}

    virtual void authAddKeyToChainResponse( FixedBuffer const &request ) {}

    virtual void authDeleteKeyFromChainResponse( FixedBuffer const &request ) {}

    virtual void authGetKeychainListResponse( FixedBuffer const &request ) {}

    virtual void authGetIdentityResponse( FixedBuffer const &request ) {}

    virtual void authAddTokenResponse( FixedBuffer const &request ) {}

    virtual void authDeleteTokenResponse( FixedBuffer const &request ) {}

    virtual void authenticateResponse( FixedBuffer const &request ) {}

    virtual void deauthenticateResponse( FixedBuffer const &request ) {}

    virtual void enableTransportSecurityResponse( FixedBuffer const &request ) {}

    virtual void disableTransportSecurityResponse( FixedBuffer const &request ) {}

    virtual void enableStreamEncryptionResponse( FixedBuffer const &request ) {}

    virtual void disableStreamEncryptionResponse( FixedBuffer const &request ) {}

    virtual void setMemoryObjectLengthResponse( FixedBuffer const &request ) {}

    virtual void getMemoryObjectLengthResponse( FixedBuffer const &request ) {}

    virtual void setStreamBackupResponse( FixedBuffer const &request ) {}

    virtual void getStreamBackupResponse( FixedBuffer const &request ) {}
};
}
