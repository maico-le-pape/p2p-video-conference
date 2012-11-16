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


#ifndef RTTREQUESTPACKET_H
#define RTTREQUESTPACKET_H

#include <iostream>
#include "parser/gttpacket.h"
#include "net/sockaddress.h"
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace Epyx;

/**
 * @brief Packet to request RTT
 **/
class RttRequestPacket : public GTTPacket {

    typedef boost::posix_time::ptime ptime;

public:
    RttRequestPacket ( const SockAddress& source, const SockAddress& destination );
    /**
     * @brief Parse GTT packet
     **/
    RttRequestPacket ( const GTTPacket& gttpkt );
    /**
     * @brief Build the raw text query for this packet
     * @sa Epyx::GTTPacket::build()
     **/
    byte_str build () const;

    SockAddress source;
    SockAddress destination;
    ptime sendingTime;

private:
    /**
     * @brief Fills the given GTT packet with information from this packet
     **/
    void fillGttPacket ( GTTPacket& gttpkt ) const;
};

/**
 * @brief Prints a short description of a RTT request packet in an output stream
 **/
std::ostream& operator<<(std::ostream& os, const RttRequestPacket& pkt); 

#endif // RTTREQUESTPACKET_H
