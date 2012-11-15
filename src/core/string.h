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
 * @file string.h
 * @brief Useful string subroutines
 */

#ifndef EPYX_STRING_H
#define EPYX_STRING_H

#include "byte.h"
#include <ctype.h>
#include <string>

namespace Epyx
{
    /**
     * @class String
     *
     * @brief Useful string functions
     */
    class String
    {
    public:
        /**
         * @brief space characters
         */
        static const char spaces[];

        /**
         * @brief ASCII CR LF
         */
        static const std::string crlf;
        /**
         * @brief ASCII CR LF, in bytes
         */
        static const byte_str crlf_bytes;

        /**
         * @brief Removes left spaces
         *
         * @return The input parameter, without spaces on its left
         */
        static std::string& ltrim(std::string& str);
        /**
         * @brief Removes right spaces
         *
         * @return The input parameter, without spaces on its right
         */
        static std::string& rtrim(std::string& str);
        /**
         * @brief Removes spaces on both sides of the string
         *
         * @return The input parameter, without spaces on its bounds
         */
        static std::string& trim(std::string& str);

        /**
         * @brief Transforms a string to uppercase
         * @param str reference to a string
         * @return str
         */
        static std::string& toUpper(std::string& str);
        /**
         * @brief Transforms a string to lowercase
         * @param str reference to a string
         * @return str
         */
        static std::string& toLower(std::string& str);

        /**
         * @brief Converts a string to an integer
         * @param str
         * @return (long)str
         * @throw ParserException if str is not an integer
         */
        static long toInt(const std::string& str);

        /**
         * @brief Converts a string to an unsigned long
         * @param str
         * @return (unisgned long)str
         * @throw ParserException if str is not an integer
         */
        static unsigned long toULong(const std::string& str);

        /**
         * @brief Converts an integer to a string
         * @param n
         * @return (string)n
         */
        static std::string fromInt(int n);
        /**
         * @brief Converts an integer to a string
         * @param n
         * @return (string)n
         */
        static std::string fromUnsignedLong(unsigned long n);

    private:
        // Forbid instanciation
        String();
    };
}

#endif /* EPYX_STRING_H */
