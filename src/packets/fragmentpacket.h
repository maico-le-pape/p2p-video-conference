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


#ifndef FRAGMENTPACKET_H
#define FRAGMENTPACKET_H

#include "parser/gttpacket.h"
#include "net/sockaddress.h"
#include <boost/date_time/posix_time/ptime.hpp>

using namespace Epyx;

class FragmentPacket : public GTTPacket {
    typedef boost::posix_time::ptime ptime;
public:
    FragmentPacket ( const byte_str& data, 
		     ptime packetTimestamp,
                     unsigned char fragmentNumber,
		     unsigned int packetSize,
		     SockAddress source );
    /**
     * @brief Parse GTT packet
     **/
    FragmentPacket ( const GTTPacket& gttpkt );
    /**
     * @brief Build the raw text query for this packet
     * @sa Epyx::GTTPacket::build()
     **/
    byte_str build() const;

    byte_str data;

    SockAddress source;
    ptime packetTimestamp;
    unsigned char fragmentNumber;
    unsigned int packetSize;

private:
    /**
     * @brief Fills the given GTT packet with information from this packet
     **/
    void fillGttPacket ( GTTPacket& gttpkt ) const;
};

#endif // FRAGMENTPACKET_H

