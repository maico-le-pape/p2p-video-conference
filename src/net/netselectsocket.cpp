#include "netselectsocket.h"
#include "../core/common.h"
#include "socket.h"

namespace Epyx
{

    NetSelectSocket::NetSelectSocket(const std::shared_ptr<Socket>& sock)
    :sock(sock) {
    }

    NetSelectSocket::NetSelectSocket(Socket *psock)
    :sock(psock) {
    }

    int NetSelectSocket::getFileDescriptor() const {
        EPYX_ASSERT(sock);
        return sock->getFd();
    }

    bool NetSelectSocket::read() {
        EPYX_ASSERT(sock);
        const int size = 4096;
        byte data[size];
        int recvSize = sock->recv(data, size);

        // Quit on End-Of-File
        if (recvSize == 0)
            return false;

        // Eat data
        try {
            this->eat(byte_str(data, recvSize));
        } catch (MinorException e) {
            log::error << e << log::endl;
            log::error << "Closing socket to " << sock->getAddress() <<
                " due to an exception" << log::endl;
            return false;
        }
        return sock->isOpened();
    }

    const std::shared_ptr<Socket>& NetSelectSocket::socket() const {
        EPYX_ASSERT(sock);
        return sock;
    }
}
