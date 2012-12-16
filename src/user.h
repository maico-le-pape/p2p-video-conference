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


#ifndef USER_H
#define USER_H

#include <string>
#include "net/sockaddress.h"
#include "net/udpsocket.h"
#include "boost/shared_ptr.hpp"
#include <queue>
#include "frame.h"
#include "webm/framepacket.h"
#include "fragmentmanager.h"
#include <QLabel>
#include <boost/date_time/posix_time/ptime.hpp>
#include <QMutex>

using namespace std;
using namespace Epyx;

class VideoConferenceP2P;

using namespace Epyx::webm;

class User {
    typedef boost::posix_time::ptime ptime;
public:
    User ( string, SockAddress, VideoConferenceP2P& vc );
    string getName() const;
    SockAddress getAddress() const;
    string getIpStr();
    unsigned short int getDelay() const;
    void updateDelay ( unsigned short int delay );
    void send(const void *data, int size);
    void receive ( FragmentPacket& fp);
    void add ( Frame* f);
    QImage getLatestFrame(ptime maxTime);

private:
    string name;
    SockAddress address;
    unsigned short int delay;
    VideoConferenceP2P& video_conference;
    priority_queue<Frame*, std::vector<Frame*>, FrameCompare> frames;
    FragmentManager fragmentManager;
    mutable QMutex mutex_delay;
};

#endif // USER_H
