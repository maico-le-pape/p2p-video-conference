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

User::User ( string s, SockAddress sa ) : socket ( new UDPSocket ( sa ) )
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
    return delay;
}

void User::updateDelay ( short unsigned int delay )
{
    this->delay = delay;
}

/**
 * @brief Send data through the socket
 *
 * @param data sent data
 * @param size size of data to be sent
 * @return number of sent bytes
 */
void User:: send ( const void *data, int size )
{
    socket->send ( data, size );
}

string User::getIpStr()
{
  return address.getIpStr();
}
