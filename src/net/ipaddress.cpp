#include "ipaddress.h"
#include "../core/common.h"
#include <sstream>
#include <string.h>
#include <ifaddrs.h>

namespace Epyx
{

    IpAddress::IpAddress()
    :ip(), ipVersion(0) {
    }

    IpAddress::IpAddress(const std::string& ip, int ipVersion)
    :ip(ip), ipVersion(ipVersion) {
        std::string portstr = "";
        if (ip.empty())
            return;
        size_t iplen = ip.length();

        // Remove brackets around IPv6 addresses
        if (ip.at(0) == '[') {
            this->ipVersion = 6;
            if (ip.at(iplen - 1) != ']') {
                // TODO: Better sanity checks
                log::fatal << "Unknown IPv6 " << ip << log::endl;
                throw ParserException("IpAddress", "Invalid IPv6 address");
            }
            this->ip = ip.substr(1, iplen - 2);
        }

        // Guess IP version
        if (this->ipVersion == 0) {
            if (ip.find(':') != std::string::npos)
                this->ipVersion = 6;
            else
                this->ipVersion = 4;
        }

        this->simplifyIP();
    }

    IpAddress::IpAddress(const struct sockaddr *saddr) {
        EPYX_ASSERT(saddr != NULL);
        char ipBuffer[INET6_ADDRSTRLEN + 1];

        if (saddr->sa_family == AF_INET) {
            // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) saddr;
            inet_ntop(AF_INET, &(ipv4->sin_addr), ipBuffer, sizeof (ipBuffer));
            this->ip.assign(ipBuffer);
            this->ipVersion = 4;
        } else if (saddr->sa_family == AF_INET6) {
            // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) saddr;
            inet_ntop(AF_INET6, &(ipv6->sin6_addr), ipBuffer, sizeof (ipBuffer));
            this->ip.assign(ipBuffer);
            this->ipVersion = 6;
        } else {
            log::fatal << "You have just invented a new IP version " \
                << "without giving me information about how to handle it\n" \
                << "The version is: " << saddr->sa_family << "\n" \
                << "IPv4 is: " << AF_INET << "\n" \
                << "IPv6 is: " << AF_INET6 \
                << log::endl;
            throw FailException("IpAddress", "Bad IP Version");
        }
    }

    int IpAddress::getVersion() const {
        return ipVersion;
    }

    bool IpAddress::empty() const {
        return ip.empty();
    }

    void IpAddress::getSockAddr(struct sockaddr *saddr, unsigned short port) const {
        EPYX_ASSERT(saddr != NULL);
        EPYX_ASSERT(this->ipVersion > 0);
        if (this->ipVersion == 4) {
            // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) saddr;
            memset(&(ipv4->sin_zero), 0, sizeof (ipv4->sin_zero));
            ipv4->sin_family = AF_INET;
            inet_pton(AF_INET, ip.c_str(), &(ipv4->sin_addr));
            ipv4->sin_port = htons(port);
        } else if (this->ipVersion == 6) {
            // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) saddr;
            ipv6->sin6_family = AF_INET6;
            ipv6->sin6_flowinfo = 0;
            ipv6->sin6_scope_id = 0;
            inet_pton(AF_INET6, ip.c_str(), &(ipv6->sin6_addr));
            ipv6->sin6_port = htons(port);
        } else {
            throw FailException("IpAddress", "Bad IP Version");
        }
    }

    std::ostream& operator<<(std::ostream& os, const IpAddress& addr) {
        if (addr.ip.length())
            os << addr.ip;
        else
            os << '*';
        return os;
    }

    std::string IpAddress::toString(bool addBrackets) const {
        if (addBrackets && ipVersion == 6) {
            std::ostringstream stream;
            stream << '[' << ip << ']';
            return stream.str();
        } else {
            return ip;
        }
    }

    bool IpAddress::isLocal() const {
        if (ipVersion == 0 || ipVersion == 4) {
            // 127.0.0.0/8
            if (ip.length() > 4 && !ip.substr(0, 4).compare("127."))
                return true;
        }
        if (ipVersion == 0 || ipVersion == 6) {
            // ::*
            if (ip.length() > 2 && !ip.substr(0, 2).compare("::"))
                return true;
        }
        return false;
    }

    int IpAddress::compare(const IpAddress& addr) const {
        if (ipVersion < addr.ipVersion) return -1;
        if (ipVersion > addr.ipVersion) return 1;
        // TODO: compare with bitmasks
        int c = ip.compare(addr.ip);
        if (c != 0) return c;
        return 0;
    }

    bool operator==(const IpAddress& addr1, const IpAddress& addr2) {
        return addr1.compare(addr2) == 0;
    }

    bool operator!=(const IpAddress& addr1, const IpAddress& addr2) {
        return addr1.compare(addr2) != 0;
    }

    bool operator<(const IpAddress& addr1, const IpAddress& addr2) {
        return addr1.compare(addr2) < 0;
    }

    void IpAddress::simplifyIP() {
        sockaddr_storage saddr;
        this->getSockAddr((sockaddr*)&saddr, 0);
        IpAddress newAddr((sockaddr*)&saddr);
        ip = newAddr.ip;
    }
}
