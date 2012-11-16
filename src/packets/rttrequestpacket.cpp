/*
    Copyright 2012 <copyright holder> <email>

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/


#include "rttrequestpacket.h"

#include <core/log.h>
#include <boost/algorithm/string.hpp>

RttRequestPacket::RttRequestPacket ( const SockAddress& source,
                                     const SockAddress& destination ) :
    source ( source ), destination ( destination ),
    sendingTime ( boost::posix_time::microsec_clock::local_time() )
{
}

RttRequestPacket::RttRequestPacket ( const GTTPacket& gttpkt )
{
    // Check protocol
    if ( gttpkt.protocol.compare ( "VCP2P" ) ) {
        log::error << "RttRequest: Incorrect GTT protocol" << gttpkt.protocol
                   << log::endl;
        throw new ParserException ( "RttRequestPacket", "Invalid RTT request "
                                    "packet" );
    }

    if ( gttpkt.method.compare ( "RTTREQ" ) ) {
        log::error << "RttRequest: Incorrect GTT method" << gttpkt.method
                   << log::endl;
        throw new ParserException ( "RttRequestPacket", "Invalid RTT request "
                                    " packet" );
    }

    // Parse headers
    for ( auto it = gttpkt.headers.begin(); it != gttpkt.headers.end(); it++ ) {
        if ( boost::iequals ( it->first, "Source" ) )
            source = SockAddress ( it->second );

        if ( boost::iequals ( it->first, "Destination" ) )
            destination = SockAddress ( it->second );

        if ( boost::iequals ( it->first, "Time" ) )
            boost::posix_time::time_from_string ( it->second );
    }
}

byte_str RttRequestPacket::build () const
{
    GTTPacket gttpkt;
    fillGttPacket ( gttpkt );
    return gttpkt.build();
}

void RttRequestPacket::fillGttPacket ( GTTPacket& gttpkt ) const
{
    gttpkt.protocol = "VCP2P";
    gttpkt.method = "RTTREQ";
    gttpkt.headers["Source"] = source.toString();
    gttpkt.headers["Destination"] = destination.toString();
    gttpkt.headers["Time"] = boost::posix_time::to_simple_string ( sendingTime
                                                                 );
}

std::ostream& operator<< ( std::ostream& os, const RttRequestPacket& pkt )
{
    os << "RTT Request from " << pkt.source.toString()
       << " to " << pkt.destination.toString()
       << " with sending time "
       << boost::posix_time::to_simple_string ( pkt.sendingTime );

    return os;
}
