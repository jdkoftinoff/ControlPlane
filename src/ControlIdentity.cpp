#include "ControlPlane/World.hpp"
#include "ControlPlane/ControlIdentity.hpp"

namespace ControlPlane
{

int compare( const ControlIdentity &lhs, const ControlIdentity &rhs )
{
    int r = 1;

    if ( lhs.m_descriptor_type == rhs.m_descriptor_type )
    {
        if ( lhs.m_descriptor_index == rhs.m_descriptor_index )
        {
            if ( lhs.m_section == rhs.m_section )
            {
                if ( lhs.m_item == rhs.m_item )
                {
                    if ( lhs.m_w_pos == rhs.m_w_pos )
                    {
                        if ( lhs.m_h_pos == rhs.m_h_pos )
                        {
                            r = 0;
                        }
                        else if ( lhs.m_h_pos < rhs.m_h_pos )
                        {
                            r = -1;
                        }
                    }
                    else if ( lhs.m_w_pos < rhs.m_w_pos )
                    {
                        r = -1;
                    }
                }
                else if ( lhs.m_item < rhs.m_item )
                {
                    r = -1;
                }
            }
            else if ( lhs.m_section < rhs.m_section )
            {
                r = -1;
            }
        }
        else if ( lhs.m_descriptor_index < rhs.m_descriptor_index )
        {
            r = -1;
        }
    }
    else if ( lhs.m_descriptor_type < rhs.m_descriptor_type )
    {
        r = -1;
    }

    return r;
}
}
