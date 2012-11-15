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
 * @file uri.h
 * @brief Uniform Resource Identifier
 */

#ifndef EPYX_URI_H
#define EPYX_URI_H

#include <string>
#include "sockaddress.h"

namespace Epyx
{
    /**
     * @class URI
     *
     * @brief URI Storage
     *
     * An URI is a string in the form of "http://192.168.42.1:1337/path/to/file"
     * This is internally splitted into
     * - a scheme (http)
     * - an Address (IP:port)
     * - an absolute path (/path/to/file)
     */
    class URI
    {
    public:
        /**
         * @brief Empty constructor
         */
        URI();
        /**
         * @brief Full constructor
         * @param scheme
         * @param addr
         * @param path
         */
        URI(const std::string& scheme, const SockAddress& addr, const std::string& path);
        /**
         * @brief Constructor with parsing
         * @param uri
         */
        URI(const std::string& uri);
        /**
         * @brief Copy constructor
         * @param uri
         */
        URI(const URI& uri);
        /**
         * @brief Assignment constructor
         * @param uri
         */
        URI& operator=(const URI& uri);

        /**
         * @brief Get scheme
         * @return this->scheme
         */
        const std::string& getScheme() const;
        /**
         * @brief Get address
         * @return this->addr
         */
        const SockAddress& getAddress() const;
        /**
         * @brief Get absolute path
         * @return this->path
         */
        const std::string& getPath() const;

        /**
         * @brief Print the address in an output stream
         * @param os output stream
         * @param uri
         */
        friend std::ostream& operator<<(std::ostream& os, const URI& uri);

        /**
         * @brief get a std::string representation of the URI
         */
        std::string toString() const;

    private:
        std::string scheme;
        SockAddress addr;
        std::string path;
    };
}

#endif /* EPYX_URI_H */

