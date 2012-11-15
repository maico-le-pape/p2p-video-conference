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
 * @file netif.h
 * @brief Network interface management
 */

#ifndef EPYX_NET_NETIF_H
#define EPYX_NET_NETIF_H

#include <sys/types.h>
#include <ifaddrs.h>
#include <vector>
#include "ipaddress.h"

namespace Epyx
{
    class NetIf
    {
    public:
        NetIf();
        /**
         * @brief Build a NetIf from a result of getifaddrs
         * @param iface
         */
        NetIf(const struct ifaddrs& iface);

        friend std::ostream& operator<<(std::ostream& os, const NetIf& ni);

        const std::string& getName() const;

        const IpAddress& getAddress() const;

        const IpAddress& getBroadcast() const;

        bool isUp() const;

        /**
         * @brief Get all interfaces on the system
         */
        static std::vector<NetIf> getAllNet();

        /**
         * @brief Get all interfaces for a given name
         * @param name interface's name
         */
        static std::vector<NetIf> getByName(const std::string& name);

        /**
         * @brief Find the first external interface, or a local one if no one is found.
         * @param ipVersion which IP version to get (4 or 6)
         * @param doLog print info-log messages
         * @return NetIf to the found interface
         * @throw MinorException if no interface is found
         *
         * On common system, there is always a loopback interface so this function
         * may rarely throw an exception.
         */
        static NetIf pickExternalInterface(int ipVersion, bool doLog = false);

    private:
        // These properties are copies of struct ifaddrs, see man 3 getifaddrs
        std::string name;
        unsigned int flags;
        IpAddress addr;
        IpAddress netmask;
        IpAddress broadcast;
    };
}

#endif /* IOM_NET_NETIF_H */

