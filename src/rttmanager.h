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


#ifndef RTTMANAGER_H
#define RTTMANAGER_H

#include "packets/rttreplypacket.h"
#include "core/thread.h"

class VideoConferenceP2P;

using namespace Epyx;

class RTTManager : public Thread {

public:
    RTTManager ( VideoConferenceP2P* vc );
    void processRTT ( const RttReplyPacket& packet );
    void run();
    unsigned int getMaxDelay() const;
    static const unsigned int threshold = 1700;

private:
    VideoConferenceP2P* conference;
    unsigned int maxDelay = 0;
};

#endif // RTTMANAGER_H
