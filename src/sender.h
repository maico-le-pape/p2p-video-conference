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


#ifndef SENDER_H
#define SENDER_H

#include "core/thread.h"
#include "core/log.h"

class VideoConferenceP2P;

using namespace Epyx;

class Sender : public Thread {

public:
    Sender(VideoConferenceP2P* vc);
    void run();

private:
  VideoConferenceP2P* conference;
};

#endif // SENDER_H
