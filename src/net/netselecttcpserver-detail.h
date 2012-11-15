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
#include "netselect.h"
#include "../core/common.h"
#include "server.h"
#include "socket.h"

namespace Epyx
{

    template<class T, typename TP>
    NetSelectTCPServer<T, TP>::NetSelectTCPServer(TCPServer *srv, TP param)
    : srv(srv), param(param) {
        EPYX_ASSERT(srv != NULL);
        // Force bind
        if (!srv->isBinded())
            throw FailException("NetSelectTCPServer", "Unbinded server");
    }

    template<class T, typename TP> int NetSelectTCPServer<T, TP>::getFileDescriptor() const {
        EPYX_ASSERT(srv);
        return srv->getFd();
    }

    template<class T, typename TP> bool NetSelectTCPServer<T, TP>::read() {
        EPYX_ASSERT(srv);
        struct sockaddr_storage clientAddr;
        socklen_t clientAddrLen;
        int newfd = -1;

        clientAddrLen = sizeof clientAddr;
        int srvfd = srv->getFd();
        if (srvfd < 0)
            return false;
        newfd = ::accept(srvfd, (struct sockaddr*) &clientAddr,
                &clientAddrLen);
        if (newfd == -1) {
            if (!srv->isBinded())
                return false;
            throw ErrException("NetSelectTCPServer::read", "accept");
        }

        // Encapsulate socket
        std::shared_ptr<TCPSocket> newSock(new TCPSocket(newfd, SockAddress((struct sockaddr*) &clientAddr)));
        if (newSock->getLocalAddress().empty())
            newSock->setLocalAddress(srv->getAddress());

        try {
            std::shared_ptr<T> nsSocket(new T(newSock, param));
            this->getOwner()->add(nsSocket);
        } catch (std::exception e) {
            log::error << "Unable to setup the link:\n" << e.what() << log::endl;
            return true;
        }
        return true;
    }
}
