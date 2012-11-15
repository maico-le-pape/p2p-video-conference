/*
 *   Copyright 2012 Epyx Team
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
/**
 * @file gttpacket.h
 * @brief Packet of the Generic Text Template
 */

#ifndef EPYX_GTTPACKET_H
#define EPYX_GTTPACKET_H

#include <map>
#include <string>
#include <ostream>
#include "../core/byte.h"
#include "../net/socket.h"

namespace Epyx
{
    /**
     * @class GTTPacket
     *
     * @brief Packet for Generic Text Template
     */
    class GTTPacket
    {
    public:
        /**
         * @brief Print a short packet description in an output stream (for debug)
         * @param os output stream
         * @param pkt
         */
        friend std::ostream& operator<<(std::ostream& os, const GTTPacket& pkt);

        /**
         * @brief Build the raw text query for this GTT Packet
         * @return bytes string
         */
        byte_str build() const;

        /**
         * @brief Protocol name
         */
        std::string protocol;
        /**
         * @brief Method name
         */
        std::string method;
        /**
         * @brief Headers map
         */
        std::map<std::string, std::string> headers;
        /**
         * @brief Content body
         */
        byte_str body;
    };
}

#endif /* EPYX_GTTPACKET_H */
