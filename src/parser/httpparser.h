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
 * @file httpparser.h
 * @brief HTTP Parser
 */

#ifndef EPYX_HTTPPARSER_H
#define EPYX_HTTPPARSER_H

#include "gttparser.h"

namespace Epyx
{
    /**
     * @class HTTPParser
     *
     * @brief HTTP implementation
     */
    class HTTPParser : public GTTParser {
    private:
        /**
         * @brief Parse the first line of HTTP
         * @param line first line
         * @throw ParserException on errors
         */
        void parseFirstLine(const std::string& line);

        /**
         * @brief Parse a header line of HTTP
         * @param line header line
         * @throw ParserException on errors
         */
        void parseHeaderLine(const std::string& line);

    };
}

#endif /* EPYX_HTTPPARSER_H */

