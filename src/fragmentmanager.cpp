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


#include "fragmentmanager.h"

#include <boost/date_time/posix_time/posix_time.hpp>

FragmentManager::FragmentManager() :
    packets ( 4 )
{
}

Frame* FragmentManager::eat ( FragmentPacket& fp )
{
    for ( auto it = packets.begin(); it != packets.end(); it++ )
        if ( it->packetTimestamp == fp.packetTimestamp ) {
            it->addFragment ( fp );

            if ( it->isComplete() )
                return readPacket ( it->getData() , fp.packetTimestamp );
            else
                return nullptr;
        }

    packets.push_back ( FragmentList ( fp.packetSize, fp.packetTimestamp ) );
    packets.back().addFragment ( fp );

    if ( packets.back().isComplete() )
        return readPacket ( packets.back().getData() , fp.packetTimestamp );
    else
        return nullptr;
}

Frame* FragmentManager::readPacket ( const byte_str& data,
FragmentManager::ptime time )
{
    return new Frame();
}

std::vector< FragmentPacket > FragmentManager::cut ( unsigned char* data,
        ssize_t size )
{
    unsigned int nbOfPackets = size / 1500 + ( size % 1500 == 0 ? 0 : 1 );
    std::vector<FragmentPacket> res;

    for ( int i = 0; i < nbOfPackets; i++ ) {
        unsigned char* fragmentData =
            new unsigned char[i == nbOfPackets - 1 ? size % 1500 : 1500];
        memcpy ( fragmentData, data + 1500 * i,
                 i == nbOfPackets - 1 ? size % 1500 : 1500 );
        res.insert(res.end(), FragmentPacket ( byte_str ( fragmentData, i ==
nbOfPackets - 1
                                             ? size % 1500 : 1500 ),
		boost::posix_time::microsec_clock::local_time(), i, size,
                                  SockAddress() ));
    }
    return res;
}
