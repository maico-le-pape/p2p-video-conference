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


#include "rttreplypacket.h"

#include <core/log.h>
#include <boost/algorithm/string.hpp>

RttReplyPacket::RttReplyPacket ( const SockAddress& source,
                                 const SockAddress& destination,
                                 const ptime& sendingTime ) :
    source ( source ), destination ( destination ), sendingTime ( sendingTime )
{
}

RttReplyPacket::RttReplyPacket ( const GTTPacket& gttpkt )
{
    // Check protocol
    if ( gttpkt.protocol.compare ( "VCP2P" ) ) {
        log::error << "RttReply: Incorrect GTT protocol" << gttpkt.protocol
                   << log::endl;
        throw new ParserException ( "RttRequestPacket", "Invalid RTT request "
                                    "packet" );
    }

    if ( gttpkt.method.compare ( "RTTREP" ) ) {
        log::error << "RttReply: Incorrect GTT method" << gttpkt.method
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
            sendingTime = boost::posix_time::time_from_string ( it->second );
    }
}

byte_str RttReplyPacket::build () const
{
    GTTPacket gttpkt;
    fillGttPacket ( gttpkt );
    return gttpkt.build();
}

void RttReplyPacket::fillGttPacket ( GTTPacket& gttpkt ) const
{
    gttpkt.protocol = "VCP2P";
    gttpkt.method = "RTTREP";
    gttpkt.headers["Source"] = source.toString();
    gttpkt.headers["Destination"] = destination.toString();
    gttpkt.headers["Time"] = boost::posix_time::to_simple_string ( sendingTime
                                                                 );
}

std::ostream& operator<< ( std::ostream& os, const RttReplyPacket& pkt )
{
    os << "RTT Reply from " << pkt.source.toString()
       << " to " << pkt.destination.toString()
       << " with sending time "
       << boost::posix_time::to_simple_string ( pkt.sendingTime );

    return os;
}
