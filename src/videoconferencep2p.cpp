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
#include <boost/assert.hpp>

VideoConferenceP2P::VideoConferenceP2P ( SockAddress sa )
{
}

void VideoConferenceP2P::add ( string u_name, SockAddress sa )
{
    users.insert ( std::pair<SockAddress, User> ( sa, User ( u_name, sa ) ) );
}

User& VideoConferenceP2P::getUser ( SockAddress address )
{
    auto it = users.find(address);
    BOOST_ASSERT(it != users.end());
    return it->second;
}
