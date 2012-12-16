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


#ifndef RECEIVER_H
#define RECEIVER_H

#include "net/udpserver.h"
#include "net/sockaddress.h"
#include "boost/shared_ptr.hpp"
#include "core/thread.h"

class VideoConferenceP2P;

using namespace Epyx;

class Receiver : public Thread {

public:
    Receiver ( VideoConferenceP2P* vc );
    void run();
    void setDisplay( bool d);

private:
    VideoConferenceP2P* conference;
    bool display = true;
};

#endif // RECEIVER_H
