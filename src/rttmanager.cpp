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


#include "rttmanager.h"
#include "videoconferencep2p.h"
#include "packets/rttrequestpacket.h"
#include "core/log.h"

RTTManager::RTTManager ( VideoConferenceP2P* vc ) :
    conference ( vc )
{

}

void RTTManager::processRTT ( const RttReplyPacket& packet )
{
    unsigned int delay = ( boost::posix_time::microsec_clock::local_time() -
                           packet.sendingTime ).total_milliseconds() / 2;


    if ( delay > maxDelay && delay < threshold ) {
        unsigned int previousDelay =
            conference->getUser ( packet.source ).getDelay();
        if ( previousDelay == maxDelay ) {
            const map< SockAddress, User >& users = conference->getUsers();
            maxDelay = 0;
            for ( auto dest = users.begin() ; dest != users.end(); dest++ ) {
                unsigned int d = dest->second.getDelay();
                if ( d < threshold ) {
                    maxDelay = std::max<int> ( maxDelay, dest->second.getDelay()
                                             );
                }
            }
           /* Epyx::log::debug << "Max delay updated " <<
                             previousDelay << " => "<<
                             maxDelay <<
                             Epyx::log::endl; */
        }
    }

    conference->updateDelay ( packet.source, delay );

    /* Epyx::log::debug << "RTT update for " <<
                     packet.source.getPort() <<
                     " set to "<<
                     delay << Epyx::log::endl; */
}

void RTTManager::run()
{
    while ( true ) {
        map< SockAddress, User > users = conference->getUsers();
        for ( auto dest = users.begin() ; dest != users.end(); dest++ ) {
            if ( conference->host != dest->first ) {

                RttRequestPacket rp ( conference->host,dest->first );
                const byte_str packet = rp.build();

                //Epyx::log::debug << rp << Epyx::log::endl;
                dest->second.send ( packet.data() , packet.length() );
            }
        }
        sleep ( 2 );
    }
}
