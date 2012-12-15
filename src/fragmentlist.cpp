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

FragmentList::FragmentList()
{
}

FragmentList::FragmentList ( unsigned int packetSize,
			     ptime packetTimestamp ) :
    packetTimestamp(packetTimestamp), data(packetSize, ' ')
{
    for(int i = 0; i < packetSize/1500 + (packetSize%150 != 0?1:0); i++)
	missingPackets.insert(i);
}

void FragmentList::addFragment ( const FragmentPacket& p )
{
    data.replace(1500*p.fragmentNumber, p.data.size(), p.data);
    missingPackets.erase(p.fragmentNumber);
}

bool FragmentList::isComplete() const
{
    return missingPackets.empty();
}

byte_str FragmentList::getData() const
{
    return data;
}