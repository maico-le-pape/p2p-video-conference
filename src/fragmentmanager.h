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


#ifndef FRAGMENTMANAGER_H
#define FRAGMENTMANAGER_H

#include "frame.h"
#include "packets/fragmentpacket.h"
#include "fragmentlist.h"
#include <parser/gttparser.h>
#include <list>

class FragmentManager {
    typedef boost::posix_time::ptime ptime;
public:
    FragmentManager();
    Frame* eat ( FragmentPacket& fp );
    static std::vector<FragmentPacket> cut ( char* data, unsigned int size );

private:
    std::list< FragmentList > packets;
    Frame* readPacket ( const byte_str& data, ptime time );
};

#endif // FRAGMENTMANAGER_H
