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

RTTManager::RTTManager ( VideoConferenceP2P& conference ) :
    _conference ( conference )
{

}

void RTTManager::processRTT ( const RttReplyPacket& packet )
{
    int delay = ( boost::posix_time::microsec_clock::local_time() -
                  packet.sendingTime ).total_milliseconds() / 2;
    _conference.getUser ( packet.source ).updateDelay ( delay );
}
