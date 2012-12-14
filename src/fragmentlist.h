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


#ifndef FRAGMENTLIST_H
#define FRAGMENTLIST_H

#include <set>
#include "packets/fragmentpacket.h"

class FragmentList {
    typedef boost::posix_time::ptime ptime;
public:
    FragmentList(unsigned int packetSize, ptime packetTimestamp);
    ~FragmentList();
    void addFragment(const FragmentPacket &p);
    bool isComplete() const;
    byte_str getData() const;
    ptime packetTimestamp;
    
private:
    std::set<unsigned char> missingPackets;
    unsigned char* data;
    unsigned int packetSize;
};

#endif // FRAGMENTLIST_H
