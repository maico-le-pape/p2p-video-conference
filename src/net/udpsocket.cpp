#include "udpsocket.h"
#include "../core/common.h"

namespace Epyx
{

    UDPSocket::UDPSocket() {
    }

    UDPSocket::UDPSocket(const SockAddress& addr, bool isBroadcast)
    :Socket(addr) {
        if (isBroadcast)
            this->setBroadcast(true);
    }

    UDPSocket::UDPSocket(int sock, const SockAddress &addr)
    :Socket(sock, addr) {
    }

    void UDPSocket::create() {
        if (sock >= 0) {
            // Do nothing, it already exists
            return;
        }
        sock = ::socket(AF_INET, SOCK_DGRAM, 0);
        if (sock == -1)
            throw ErrException("UDPSocket", "socket");
        EPYX_VERIFY(sock >= 0);
    }

    void UDPSocket::setBroadcast(bool enable) {
        if (sock < 0) {
            this->create();
        }
        int boolean = (enable ? 1 : 0);
        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolean, sizeof (boolean))) {
            throw ErrException("UDPSocket", "setsokopt(broadcast)");
        }
    }

    int UDPSocket::send(const void *data, int size) {
        int bytes;
        struct sockaddr_storage saddr;

        EPYX_ASSERT(data != NULL);
        // Create a new socket if it does not exist
        if (sock < 0) {
            this->create();
        }
        address.getSockAddr((struct sockaddr *) &saddr);
        bytes = ::sendto(sock, data, size, 0, (const struct sockaddr *) &saddr, sizeof (saddr));
        if (localAddress.empty())
            this->updateLocalAddress();
        // TODO: Implement status error (ex. Conn closed, ...)

        if (bytes < 0)
            throw ErrException("UDPSocket", "sendto");
        return bytes;
    }

    int UDPSocket::recv(void * data, int size) {
        int bytes;
        struct sockaddr_storage saddr;
        socklen_t length = sizeof (saddr);
        EPYX_ASSERT(data != NULL);
        EPYX_ASSERT(this->sock >= 0);
        bytes = ::recvfrom(this->sock, data, size, 0, (struct sockaddr *) &saddr, &length);
        // TODO: Implement status error (eg. Conn closed, ...)
        if (bytes < 0)
            throw ErrException("UDPSocket", "recvfrom");

        if (bytes == 0) {
            //Socket is now closed
            EPYX_VERIFY(sock < 0);
            return 0;
        }

        /**
         * recv doesn't set the after-the-last byte to zero. We must do it to
         * avoid some issues.
         * (writing into a prefilled longer data buffer fucks everything up)
         */
        if (bytes < size)
            ((char*) data)[bytes] = 0;
        lastRecvAddr = SockAddress((const sockaddr *) &saddr);
        return bytes;
    }

    SockAddress UDPSocket::getLastRecvAddr() const {
        return lastRecvAddr;
    }


    void UDPSocket::bindToDevice(const std::string& devicename) {
        if (sock < 0) {
            this->create();
        }
        if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE,
            devicename.c_str(), devicename.length() + 1) < 0)
            throw ErrException("UDPSocket", "setsockopt(SO_BINDTODEVICE)");
    }
}
