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


#include "fragmentlist.h"
#include <string.h>

FragmentList::FragmentList ( unsigned int packetSize,
			     ptime packetTimestamp ) :
    packetTimestamp(packetTimestamp), packetSize(packetSize)
{
    for(int i = 0; i < packetSize/1500 + (packetSize%1500 != 0?1:0); i++)
	missingPackets.insert(i);
    data = new unsigned char[packetSize];
}

FragmentList::~FragmentList()
{
    delete[] data;
}

void FragmentList::addFragment ( const FragmentPacket& p )
{
    memcpy(data + 1500 * p.fragmentNumber, p.data.data(), p.data.size());
    missingPackets.erase(p.fragmentNumber);
}

bool FragmentList::isComplete() const
{
    return missingPackets.empty();
}

byte_str FragmentList::getData() const
{
    return byte_str(data, packetSize);
}