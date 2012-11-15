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
 * @file ipaddress.h
 * @brief IP address
 */

#ifndef EPYX_NET_IPADDRESS_H
#define EPYX_NET_IPADDRESS_H

#include <iostream>
#include <string>
#include <arpa/inet.h>

namespace Epyx
{
    /**
     * @class IpAddress
     *
     * @brief Manage IP address
     *
     * This class manage both IPv4 et IPv6 address
     */
    class IpAddress
    {
    public:
        IpAddress();

        /**
         * @brief Constructor by parsing a string
         * @param ip
         * @param ipVersion
         */
        IpAddress(const std::string& ip, int ipVersion = 0);
        /**
         * @brief Constructor with a sockaddr structure
         * @param saddr a sockaddr{_in,_in6,...} pointer
         */
        IpAddress(const struct sockaddr *saddr);

        /**
         * @brief Fill in a sockaddr struct, without the port
         * @param saddr
         * @param port (optional) transport port
         */
        void getSockAddr(struct sockaddr *saddr, unsigned short port = 0) const;

        /**
         * @return IP version
         */
        int getVersion() const;

        /**
         * @brief Is it empty ?
         * @return the answer
         */
        bool empty() const;

        /**
         * @brief Print the address in an output stream
         * @param os output stream
         * @param addr
         */
        friend std::ostream& operator<<(std::ostream& os, const IpAddress& addr);

        /**
         * @brief get a std::string representation of the IP address
         */
        std::string toString(bool addBrackets = false) const;

        /**
         * Tell wether or not this IP address is local (127.0.0.0/8 or ::/16)
         */
        bool isLocal() const;

        /**
         * @brief compare this and addr
         * @param addr
         */
        int compare(const IpAddress& addr) const;

        /**
         * @brief Equality test
         * @param addr1
         * @param addr2
         */
        friend bool operator==(const IpAddress& addr1, const IpAddress& addr2);

        /**
         * @brief Inequality test
         * @param addr1
         * @param addr2
         */
        friend bool operator!=(const IpAddress& addr1, const IpAddress& addr2);

        /**
         * @brief Less test
         * @param addr1
         * @param addr2
         */
        friend bool operator<(const IpAddress& addr1, const IpAddress& addr2);

    private:
        // Rewrite IP address to a more simplier form
        void simplifyIP();

        std::string ip;
        // 4 or 6
        int ipVersion;
    };
}

#endif /* IOM_NET_ADDRESS_H */

