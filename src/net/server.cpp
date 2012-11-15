#include "server.h"
#include "../core/common.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

namespace Epyx
{

    Server::Server(const SockAddress& addr)
    :address(addr), sockfd(-1) {
    }

    Server::~Server() {
        //log::debug << "close server" << log::endl;
        // Close socket
        this->close();
    }

    void Server::close() {
        if (sockfd >= 0) {
            ::shutdown(sockfd, SHUT_RDWR);
            ::close(sockfd);
            sockfd = -1;
        }
    }

    bool Server::isBinded() const {
        return (sockfd >= 0);
    }

    const SockAddress& Server::getAddress() const {
        return address;
    }

    int Server::getFd() const {
        return sockfd;
    }

    bool Server::_internal_bind(int socktype) {
        //EPYX_ASSERT(port < 65536);
        char charport[10];
        struct addrinfo hints, *addrAll, *pai;
        int status, flag;

        // Get IP address (if empty, use NULL)
        const std::string ipaddrStr = address.getIpStr();
        const char *ipaddr = (ipaddrStr.empty() ? NULL : ipaddrStr.c_str());
        // Convert port to char* to find address hints
        snprintf(charport, sizeof (charport), "%u", address.getPort());
        memset(&hints, 0, sizeof hints);
        // AF_INET or AF_INET6 to force IP version
        hints.ai_family = AF_UNSPEC;
        // SOCK_STREAM (TCP) or SOCK_DGRAM (UDP)
        hints.ai_socktype = socktype;
        // Server = passive
        hints.ai_flags = AI_PASSIVE;
        status = ::getaddrinfo(ipaddr, charport, &hints, &addrAll);
        if (status != 0) {
            log::fatal << "getaddrinfo error " << status << ": "
                << gai_strerror(status) << log::endl;
            throw FailException("TCPServer::bind", "getaddrinfo error");
        }

        // getaddrinfo() returns a list of address structures.
        // Try each address until we successfully bind.
        for (pai = addrAll; pai != NULL; pai = pai->ai_next) {
            // Create a new socket
            this->sockfd = ::socket(pai->ai_family, pai->ai_socktype,
                pai->ai_protocol);
            if (this->sockfd == -1)
                continue;

            // Allow reusing the address (ignore failure)
            flag = 1;
            setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof (int));

            // Bind to a listening address/port
            status = ::bind(this->sockfd, pai->ai_addr, pai->ai_addrlen);
            if (status == 0) {
                // Success : remember used address
                this->address = SockAddress(pai->ai_addr);
                break;
            }
            log::warn << "bind(" << SockAddress(pai->ai_addr) << "): " << log::errstd << log::endl;

            // Close socket if bind fails
            ::close(this->sockfd);
            this->sockfd = -1;
        }
        // Free results
        freeaddrinfo(addrAll);
        if (pai == NULL) {
            log::debug << "Nowhere to bind." << log::endl;
            return false;
        }

        // Now there is a socket, listen to nbConn connections
        EPYX_ASSERT(sockfd >= 0);
        return true;
    }
}
