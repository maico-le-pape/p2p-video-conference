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


#include "user.h"
#include "net/udpsocket.h"
#include "videoconferencep2p.h"

User::User ( string s, SockAddress sa, VideoConferenceP2P& vc )
    : video_conference ( vc )
{
    name = s;
    address = sa;
}

string User::getName() const
{
    return name;
}

SockAddress User::getAddress() const
{
    return address;
}

short unsigned int User::getDelay() const
{
    QMutexLocker lock ( &mutex_delay );
    return delay;
}

string User::getIpStr()
{
    return address.getIpStr();
}

void User::updateDelay ( short unsigned int delay )
{
    QMutexLocker lock ( &mutex_delay );
    this->delay = delay;
}

/**
 * @brief Send data through the socket
 *
 * @param data sent data
 * @param size size of data to be sent
 * @return number of sent bytes
 */
void User:: send ( const void* data, int size )
{
    video_conference.getServer().sendTo ( address, data, size );
}

void User::receive ( FragmentPacket& fp )
{
    fragmentManager.eat ( fp );
    if ( fragmentManager.hasCompleteFrame() )
        add ( fragmentManager.getCompleteFrame() );
}

void User::add ( Frame f )
{
    f.setDelay ( delay );
    frames.push ( f );
}

QImage User::getLatestFrame ( User::ptime maxTime )
{
    QImage image;

    if ( frames.empty() )
        return image;

    while ( frames.top().getTime() < maxTime ) {
        image = frames.top().getImage();
        frames.pop();
    }

    return image;
}



