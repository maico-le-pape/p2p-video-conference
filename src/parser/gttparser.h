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
 * @file gttparser.h
 * @brief Generic Text Template implementation
 */

#ifndef EPYX_GTTPARSER_H
#define EPYX_GTTPARSER_H

#include "gttpacket.h"
#include "lineparser.h"
#include <ctype.h>
#include <memory>
#include <string>

namespace Epyx
{
    /**
     * @class GTTParser
     *
     * @brief GTT implementation
     */
    class GTTParser
    {
    public:
        /**
         * @brief Constructor
         */
        GTTParser();

        /**
         * @brief reset internal state
         */
        void reset();

        /**
         * @brief get error, if any
         * @return true if there is an error
         */
        bool getError(std::string& userErr) const;

        /**
         * @brief eat some fresh data
         * @param data data to eat
         */
        void eat(const byte_str& data);

        /**
         * @brief get the last read packet
         * @return a packet or an empty shared pointer.
         */
        std::unique_ptr<GTTPacket> getPacket();

    protected:
        /**
         * @brief Current GTT Packet
         *
         * This is protected to allow children parsers to fill this structure.
         */
        std::unique_ptr<GTTPacket> currentPkt;

        /**
         * @brief Current GTT Packet body size
         */
        size_t currentSize;

        /**
         * @brief Parse the first line of GTT
         * @param line first line
         * @throw ParserException on errors
         */
        virtual void parseFirstLine(const std::string& line);

        /**
         * @brief Parse a header line of GTT
         * @param line header line
         * @throw ParserException on errors
         */
        virtual void parseHeaderLine(const std::string& line);

    private:
        std::string errorMessage;
        bool hasError;

        // Use LineParser to parse lines
        LineParser lineParser;

        // Position in data
        enum DataPosition
        {
            dataposFirstLine,
            dataposHeaders,
            dataposContent
        } datapos;

        /**
         * @brief start a new packet, without cleaning read data
         */
        void startPacket();
    };
}

#endif /* EPYX_GTTPARSER_H */

