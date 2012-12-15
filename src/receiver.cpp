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


#include "receiver.h"
#include "parser/gttparser.h"
#include "parser/gttpacket.h"
#include "packets/rttreplypacket.h"
#include "packets/rttrequestpacket.h"
#include "core/log.h"
#include "user.h"
#include "videoconferencep2p.h"
#include "rttmanager.h"


using namespace Epyx;

Receiver::Receiver ( VideoConferenceP2P* vc ) : conference (vc)
{

}

void Receiver::run()
{
    GTTParser parser;
    int size;
    UDPServer& server = conference->getServer();
    RTTManager* rttManager = conference->getRTTManager();

    while ( true ) {
        const int MAX = 4096;
        byte* data = new byte[MAX];
        size = server.recv ( data,MAX );
        parser.eat ( byte_str ( data, size ) );

        std::unique_ptr<GTTPacket> packet;

        while ( ( packet =  parser.getPacket() ) != nullptr ) {

            if ( packet->method.compare ( "RTTREQ" ) == 0 ) {
                RttRequestPacket request ( * ( packet.get() ) );

                RttReplyPacket reply ( request.destination,
                                       request.source,
                                       request.sendingTime );
                const byte_str replyPacket = reply.build();

                Epyx::log::debug <<  reply << Epyx::log::endl;
                server.sendTo ( reply.destination,
                                replyPacket.data(),
                                replyPacket.size() );

            } else if ( packet->method.compare ( "RTTREP" ) == 0 ) {
		RttReplyPacket reply ( * (packet.get() ) );
		
		rttManager->processRTT(reply);
            } else if ( packet->method.compare ( "FRAGMENT" ) == 0 ) {
	      log::debug << "TODO: Fragment packets" << log::endl;
	      log::debug << "TODO: Fragment packets" << log::endl;
	    } else {
	      log::debug << "Error: Unrecognized packet" << log::endl;
	    }
        }
    }
}


