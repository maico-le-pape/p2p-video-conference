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

#include "videoconferencep2p.h"
#include "core/thread.h"
#include <boost/assert.hpp>
#include "rttmanager.h"
#include "core/log.h"
#include "user.h"

typedef std::pair<SockAddress, User> userEntry;

VideoConferenceP2P::VideoConferenceP2P ( SockAddress sa ) : host ( sa ),
    server ( sa ),  receiver ( this )
{
    rttManager = new RTTManager ( this );
    rttManager->setThreadName (
        "RTTManager " +
        boost::lexical_cast<std::string> ( sa.getPort() )
    );
    rttManager->start();

    receiver.setThreadName (
        "Receiver "  +
        boost::lexical_cast<std::string> ( sa.getPort() )
    );
    receiver.start();
}

void VideoConferenceP2P::add ( string u_name, SockAddress sa )
{
    users.insert ( userEntry ( sa,  User ( u_name,  sa,  *this ) ) );
}

void VideoConferenceP2P::printUsers()
{
    for ( auto dest = users.begin() ; dest != users.end(); dest++ ) {
      Epyx::log::debug << dest->first.getPort() << " => " <<
dest->second.getName() << Epyx::log::endl;
    }
}

User& VideoConferenceP2P::getUser ( SockAddress address )
{
    auto it = users.find ( address );
    BOOST_ASSERT ( it != users.end() );
    return it->second;
}

const map< SockAddress, User > VideoConferenceP2P::getUsers()
{
    return users;
}


void VideoConferenceP2P::updateDelay ( SockAddress address,
                                       short unsigned int delay )
{
    auto it = users.find ( address );
    if ( it == users.end() )
        Epyx::log::debug << "update Delay of a non existent address" <<
                         address.getPort() << Epyx::log::endl;
    it->second.updateDelay ( delay );
}

UDPServer& VideoConferenceP2P::getServer()
{
    return server;
}

RTTManager* VideoConferenceP2P::getRTTManager()
{
    return rttManager;
}



