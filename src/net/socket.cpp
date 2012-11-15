#include "socket.h"
#include "../core/common.h"
#include <iostream>
#include <cstring>
#if defined (WIN32)
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#define SOCKET_ERROR (-1)
#endif

namespace Epyx
{
    // Socket library initialisation
    bool Socket::is_init = false;

    void Socket::init() {
        if (!is_init) {
#if defined (WIN32)
            WSADATA WSAData;
            int error = WSAStartup(MAKEWORD(2, 2), &WSAData);
#endif
            is_init = true;
        }
    }

    void Socket::fini() {
        if (is_init) {
            is_init = false;
#if defined (WIN32)
            WSACleanup();
#endif
        }
    }

    Socket::Socket()
    :sock(-1) {
    }

    Socket::Socket(const SockAddress &addr)
    :sock(-1), address(addr) {
    }

    Socket::Socket(int sock, const SockAddress &addr)
    :sock(sock), address(addr) {
        EPYX_ASSERT(sock >= 0);
        this->updateLocalAddress();
    }

    Socket::~Socket() {
        this->close();
    }

    void Socket::setFd(int sockfd) {
        EPYX_ASSERT(this->sock < 0);
        EPYX_ASSERT(sockfd >= 0);
        this->sock = sockfd;
        this->updateLocalAddress();
    }

    int Socket::getFd() const {
        return sock;
    }

    bool Socket::isOpened() const {
        return (sock >= 0);
    }

    void Socket::setAddress(const SockAddress& addr) {
        address = addr;
    }

    SockAddress Socket::getAddress() const {
        return address;
    }

    void Socket::setLocalAddress(const SockAddress& addr) {
        localAddress = addr;
    }

    SockAddress Socket::getLocalAddress() const {
        return localAddress;
    }

    void Socket::updateLocalAddress() {
        EPYX_ASSERT(sock >= 0);
        struct sockaddr_storage saddr;
        socklen_t saddr_len = sizeof (saddr);
        if (::getsockname(sock, (struct sockaddr *) &saddr, &saddr_len) < 0) {
            log::warn << "Socket::updateLocalAddress: getsockname error "
                << log::errstd << log::endl;
            // Do nothing
            return;
        }
        // Update address
        localAddress = SockAddress((struct sockaddr *) &saddr);
    }

    void Socket::close() {
        //TODO Implement sanity checks on returned values of shutdown and close.
        // Do nothing if the socket is already closed
        if (sock < 0)
            return;
        ::shutdown(sock, SHUT_RDWR);
        ::close(sock);
        sock = -1;
    }

    bool Socket::sendAll(const void *data, int size) {
        int sentBytes;
        EPYX_ASSERT(data != NULL);
        while (size > 0) {
            sentBytes = this->send(data, size);
            if (!sentBytes) {
                log::error << "Socket::sendAll was unable to send " << size <<
                    " bytes" << log::endl;
                return false;
            }
            EPYX_ASSERT(sentBytes <= size);
            size -= sentBytes;
        }
        return true;
    }

    bool Socket::sendBytes(const byte_str& bytes) {
        return this->sendAll(bytes.c_str(), bytes.size());
    }

    bool Socket::write(const std::string& message) {
        // Do not send \0 character through the network
        return this->sendAll(message.c_str(), message.length());
    }

    bool Socket::recvAll(void *data, int size) {
        int recvBytes;
        EPYX_ASSERT(data != NULL);
        // TODO: stop on feof !
        while (size > 0) {
            recvBytes = this->recv(data, size);
            if (!recvBytes) {
                log::error << "Socket::recvAll was unable to receive " << size <<
                    " bytes" << log::endl;
                return false;
            }
            EPYX_ASSERT(recvBytes <= size);
            size -= recvBytes;
        }
        return true;
    }

    bool Socket::recvBytes(byte_str *data, int size) {
        EPYX_ASSERT(data != NULL);
        std::unique_ptr<byte[]> bytes(new byte[size]);
        if (!this->recvAll(bytes.get(), size)) return false;
        data->assign(bytes.get());
        return true;
    }
}
