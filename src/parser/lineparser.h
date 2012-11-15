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
 * @file lineparser.h
 * @brief Parse a stream line by line
 */

#ifndef EPYX_LINEPARSER_H
#define EPYX_LINEPARSER_H

#include "../core/byte.h"
#include <string>

namespace Epyx
{
    /**
     * @class LineParser
     *
     * @brief Cut raw data into lines
     */
    class LineParser
    {
    public:
        /**
         * @brief Constructor
         */
        LineParser();

        /**
         * @brief Reset everything
         */
        void reset();

        /**
         * @brief Push raw data into this object
         * @param data pushed data
         */
        void push(const byte_str& data);

        /**
         * @brief Pop one line
         * @param line string buffer where the line is read
         * @return true if a complete line was read
         */
        bool popLine(std::string& line);

        /**
         * @brief Pop raw data
         * @param data data to be written
         * @param size data size to write
         * @return true if there is enough internal data
         */
        bool popData(byte_str *data, size_t size);

    private:
        // Internal buffer
        byte_str buffer;
    };
}

#endif /* EPYX_LINEPARSER_H */
