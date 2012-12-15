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


#ifndef VIDEOCONFERENCEP2P_H
#define VIDEOCONFERENCEP2P_H

#include <map>
#include "net/sockaddress.h"
#include "net/udpserver.h"
#include "user.h"
#include "receiver.h"


class RTTManager;
class GUI;
class Sender;

using namespace std;
using namespace Epyx;

class VideoConferenceP2P {

public:
    void add ( string u_name, SockAddress sa );
    const SockAddress host;
    VideoConferenceP2P ( SockAddress sa );
    User& getUser ( SockAddress address );
    void updateDelay ( SockAddress address, short unsigned int delay );
    const map< SockAddress, User >& getUsers();
    UDPServer& getServer();
    RTTManager* getRTTManager();
    void printUsers();
    void start();

protected:
    char* debug;
    UDPServer server;

private:
    //void initialisation();
    map<SockAddress, User> users;
    RTTManager* rttManager;
    Receiver receiver;
    GUI* gui;
    Sender* sender;
};

#endif // VIDEOCONFERENCEP2P_H
