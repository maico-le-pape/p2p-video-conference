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


#include "fragmentpacket.h"

#include "core/log.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/time_formatters.hpp>
#include <boost/date_time/posix_time/time_parsers.hpp>

FragmentPacket::FragmentPacket ( const byte_str& data,
                                 ptime packetTimestamp,
                                 unsigned char fragmentNumber,
                                 unsigned int packetSize,
                                 SockAddress source ) :
    data ( data ),
    source ( source ),
    packetTimestamp ( packetTimestamp ),
    fragmentNumber ( fragmentNumber ),
    packetSize ( packetSize )
{
}

FragmentPacket::FragmentPacket ( const GTTPacket& gttpkt )
{
    // Check protocol
    if ( gttpkt.protocol.compare ( "VCP2P" ) ) {
        log::error << "FragmentPacket: Incorrect GTT protocol"
                   << gttpkt.protocol << log::endl;
        throw new ParserException ( "FragmentPacket", "Invalid fragment "
                                    "packet" );
    }

    if ( gttpkt.method.compare ( "FRAGMENT" ) ) {
        log::error << "FragmentPacket: Incorrect GTT method" << gttpkt.method
                   << log::endl;
        throw new ParserException ( "FragmentPacket", "Invalid fragment "
                                    "packet" );
    }

    // Parse headers
    for ( auto it = gttpkt.headers.begin(); it != gttpkt.headers.end(); it++ ) {
        if ( boost::iequals ( it->first, "Time" ) )
            packetTimestamp = boost::posix_time::time_from_string ( it->second
                                                                  );

        if ( boost::iequals ( it->first, "Number" ) )
            fragmentNumber = boost::lexical_cast<long> ( it->second );

        if ( boost::iequals ( it->first, "Size" ) )
            packetSize = boost::lexical_cast<long> ( it->second );

        if ( boost::iequals ( it->first, "Source" ) )
            source = SockAddress ( it->second );
    }

    // Store data
    data = gttpkt.body;
}

byte_str FragmentPacket::build() const
{
    GTTPacket gttpkt;
    fillGttPacket ( gttpkt );
    return gttpkt.build();
}

void FragmentPacket::fillGttPacket ( GTTPacket& gttpkt ) const
{
    gttpkt.protocol = "VCP2P";
    gttpkt.method = "FRAGMENT";
    gttpkt.headers["Time"] =
        boost::posix_time::to_simple_string ( packetTimestamp );
    gttpkt.headers["Number"] =
        boost::lexical_cast<std::string> ( (int) fragmentNumber );
    gttpkt.headers["Size"] =
        boost::lexical_cast<std::string> ( packetSize );
    gttpkt.headers["Source"] = source.toString();
    gttpkt.body = data;
}
