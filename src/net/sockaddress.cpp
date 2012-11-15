#include "sockaddress.h"
#include "../core/common.h"
#include <sstream>
#include <string.h>
#include <ifaddrs.h>

namespace Epyx
{

    SockAddress::SockAddress()
    :ip(), port(0) {
    }

    SockAddress::SockAddress(const std::string& addressFormat)
    :ip(), port(0) {
        // portstr may be a ":port" suffix
        std::string portstr = "";
        if (addressFormat.empty())
            return;
        if (addressFormat.at(0) == '[') {
            //Case if IPv6
            size_t n = addressFormat.find(']');
            if (n == std::string::npos) {
                // TODO: Better sanity checks
                log::fatal << "Unknown address " << addressFormat << log::endl;
                throw ParserException("SockAddress", "Invalid IPv6 address");
            }
            ip = IpAddress(addressFormat.substr(1, n - 1), 6);
            portstr = addressFormat.substr(n + 1);
        } else {
            //If not IPv6, it's IPv4, obviously :P
            size_t n = addressFormat.find(':');
            if (n == std::string::npos) {
                ip = IpAddress(addressFormat, 4);
            } else {
                ip = IpAddress(addressFormat.substr(0, n), 4);
                portstr = addressFormat.substr(n);
            }
        }
        // Use port
        if (!portstr.empty() && portstr[0] == ':')
            port = (unsigned short) String::toInt(portstr.substr(1));
        else
            port = 0;
    }

    SockAddress::SockAddress(const std::string& ip, unsigned short port, int ipVersion)
    :ip(ip, ipVersion), port(port) {
    }

    SockAddress::SockAddress(const IpAddress& addr, unsigned short port)
    :ip(addr), port(port) {
    }

    SockAddress::SockAddress(const struct sockaddr *saddr)
    :ip(saddr) {
        EPYX_ASSERT(saddr != NULL);

        if (saddr->sa_family == AF_INET) {
            // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) saddr;
            this->port = ntohs(ipv4->sin_port);
        } else if (saddr->sa_family == AF_INET6) {
            // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) saddr;
            this->port = ntohs(ipv6->sin6_port);
        } else {
            log::fatal << "You have just invented a new IP version " \
                << "without giving me information about how to handle it\n" \
                << "The version is: " << saddr->sa_family << "\n" \
                << "IPv4 is: " << AF_INET << "\n" \
                << "IPv6 is: " << AF_INET6 \
                << log::endl;
            throw FailException("SockAddress", "Bad IP Version");
        }
    }

    SockAddress::SockAddress(const SockAddress& addr)
    :ip(addr.ip), port(addr.port) {
    }

    SockAddress& SockAddress::operator=(const SockAddress& addr) {
        if (this != &addr) {
            this->ip = addr.ip;
            this->port = addr.port;
        }
        return *this;
    }

    const IpAddress& SockAddress::getIp() const {
        return ip;
    }

    std::string SockAddress::getIpStr() const {
        return ip.toString();
    }

    unsigned short SockAddress::getPort() const {
        return port;
    }

    bool SockAddress::empty() const {
        return ip.empty() && (port == 0);
    }

    void SockAddress::getSockAddr(struct sockaddr *saddr) const {
        EPYX_ASSERT(saddr != NULL);
        ip.getSockAddr(saddr, port);
    }

    std::ostream& operator<<(std::ostream& os, const SockAddress& addr) {
        os << addr.ip;

        if (addr.port)
            os << ':' << addr.port;
        return os;
    }

    std::string SockAddress::toString() const {
        std::ostringstream stream;
        stream << *this;
        return stream.str();
    }

    int SockAddress::compare(const SockAddress& addr) const {
        int c = ip.compare(addr.ip);
        if (c != 0) return c;
        if (port < addr.port) return -1;
        if (port > addr.port) return 1;
        return 0;
    }

    bool operator==(const SockAddress& addr1, const SockAddress& addr2) {
        return addr1.compare(addr2) == 0;
    }

    bool operator!=(const SockAddress& addr1, const SockAddress& addr2) {
        return addr1.compare(addr2) != 0;
    }

    bool operator<(const SockAddress& addr1, const SockAddress& addr2) {
        return addr1.compare(addr2) < 0;
    }

    std::vector<SockAddress> SockAddress::getIfaceAdresses(int port) {
        std::vector<SockAddress> addrs;
        struct ifaddrs *ifAddrStruct = NULL;
        struct ifaddrs *ifa = NULL;

        getifaddrs(&ifAddrStruct);
        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr->sa_family == AF_INET || ifa->ifa_addr->sa_family == AF_INET6) {
                SockAddress addr(ifa->ifa_addr);
                addr.port = port;
                addrs.push_back(addr);
            }
        }
        if (ifAddrStruct != NULL)
            freeifaddrs(ifAddrStruct);
        return addrs;
    }
}
