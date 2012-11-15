#include "netif.h"
#include "../core/common.h"
#include <sstream>
#include <sys/ioctl.h>
#include <net/if.h>


namespace Epyx
{

    NetIf::NetIf()
    :flags(0) {
        // Everything is correctly initialized by default but flags...
    }

    NetIf::NetIf(const struct ifaddrs& iface)
    :name(iface.ifa_name), flags(iface.ifa_flags) {
        if (iface.ifa_addr->sa_family == AF_INET || iface.ifa_addr->sa_family == AF_INET6) {
            if (iface.ifa_addr != NULL)
                addr = IpAddress(iface.ifa_addr);
            if (iface.ifa_netmask != NULL)
                netmask = IpAddress(iface.ifa_netmask);
        }
        if (flags & IFF_BROADCAST && iface.ifa_ifu.ifu_broadaddr != NULL) {
            broadcast = IpAddress(iface.ifa_ifu.ifu_broadaddr);
        }
    }

    std::ostream& operator<<(std::ostream& os, const NetIf& ni) {
        os << ni.name << " (" << ni.addr << "/" << ni.netmask;
        if (!ni.broadcast.empty())
            os << " bdcast " << ni.broadcast;
        os << ")";
        return os;
    }

    const std::string& NetIf::getName() const {
        return name;
    }

    const IpAddress& NetIf::getAddress() const {
        return addr;
    }

    const IpAddress& NetIf::getBroadcast() const {
        return broadcast;
    }

    bool NetIf::isUp() const {
        return flags & IFF_UP;
    }

    std::vector<NetIf> NetIf::getAllNet() {
        std::vector<NetIf> ifaces;
        struct ifaddrs *ifAddrStruct = NULL;
        struct ifaddrs *ifa = NULL;

        getifaddrs(&ifAddrStruct);
        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr != NULL
                && (ifa->ifa_addr->sa_family == AF_INET || ifa->ifa_addr->sa_family == AF_INET6)) {
                ifaces.push_back(NetIf(*ifa));
            }
        }
        if (ifAddrStruct != NULL)
            freeifaddrs(ifAddrStruct);
        return ifaces;
    }

    std::vector<NetIf> NetIf::getByName(const std::string& name) {
        std::vector<NetIf> ifaces;
        struct ifaddrs *ifAddrStruct = NULL;
        struct ifaddrs *ifa = NULL;

        getifaddrs(&ifAddrStruct);
        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
            if (name.compare(ifa->ifa_name) == 0) {
                ifaces.push_back(NetIf(*ifa));
            }
        }
        if (ifAddrStruct != NULL)
            freeifaddrs(ifAddrStruct);
        return ifaces;
    }

    NetIf NetIf::pickExternalInterface(int ipVersion, bool doLog) {
        std::vector<NetIf> ifaces = NetIf::getAllNet();
        std::vector<NetIf> externIfaces;
        std::vector<NetIf> localIfaces;

        for (std::vector<NetIf>::const_iterator iiface = ifaces.begin();
            iiface != ifaces.end(); iiface++) {
            // Keep only specific IP version
            const IpAddress& addr = iiface->getAddress();
            if (addr.empty() || addr.getVersion() != ipVersion)
                continue;
            // Difference between local and extern
            if (addr.isLocal())
                localIfaces.push_back(*iiface);
            else
                externIfaces.push_back(*iiface);
        }

        if (!externIfaces.empty()) {
            if (doLog) {
                if (externIfaces.size() >= 2) {
                    // Found more than 2 external interface, keep first
                    log::info << "More than one extern network interface found. Using first of:\n";
                    for (std::vector<NetIf>::const_iterator iiface = externIfaces.begin();
                        iiface != externIfaces.end(); iiface++) {
                        log::info << " * " << *iiface << "\n";
                    }
                    log::info << log::endl;

                } else if (doLog) {
                    // Found exactly one external interface
                    log::info << "Found extern network interface " << externIfaces[0]
                        << log::endl;
                }
            }
            return externIfaces[0];
        } else if (!localIfaces.empty()) {
            if (doLog) {
                log::info << "No extern network interface found, using local."
                    << log::endl;
                if (localIfaces.size() >= 2) {
                    // Found more than 2 external interface, keep first
                    log::info << "More than one local network interface found. Using first of:\n";
                    for (std::vector<NetIf>::const_iterator iiface = localIfaces.begin();
                        iiface != localIfaces.end(); iiface++) {
                        log::info << " * " << *iiface << "\n";
                    }
                    log::info << log::endl;

                } else if (doLog) {
                    // Found exactly one external interface
                    log::info << "Found local network interface " << localIfaces[0]
                        << log::endl;
                }
            }
            return localIfaces[0];
        }
        if (doLog) {
            log::info << "No network interface found" << log::endl;
        }
        throw MinorException("NetIf", "No network interface found");
    }
}
