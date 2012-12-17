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

FragmentManager::FragmentManager()
{
}

void FragmentManager::eat ( FragmentPacket& fp )
{
    if ( fragmentList.packetTimestamp != fp.packetTimestamp )
        fragmentList = FragmentList ( fp.packetSize, fp.packetTimestamp );

    fragmentList.addFragment ( fp );
}

bool FragmentManager::hasCompleteFrame() const
{
    return fragmentList.isComplete();
}

Frame* FragmentManager::getCompleteFrame( ) const
{
    return new Frame ( fragmentList.getData(),
	boost::posix_time::microsec_clock::local_time(),
	fragmentList.packetTimestamp
 		    );
}

std::vector< FragmentPacket > FragmentManager::cut ( char* data,
        unsigned int size )
{
    unsigned int nbOfPackets = size / 1500 + ( size % 1500 == 0 ? 0 : 1 );
    std::vector<FragmentPacket> res;

    byte_str data_str ( reinterpret_cast<unsigned char*> ( data ), size );
    boost::posix_time::ptime time =
        boost::posix_time::microsec_clock::local_time();

    for ( unsigned int i = 0; i < nbOfPackets; i++ ) {
        byte_str fragmentData;

        try {
            fragmentData = data_str.substr ( 1500 * i, 1500 );
        } catch ( std::out_of_range& e ) {
            std::cout << e.what() << std::endl;
        }

        res.insert ( res.end(), FragmentPacket ( fragmentData,
                     time, i, size,
                     SockAddress() ) );
    }

    return res;
}
