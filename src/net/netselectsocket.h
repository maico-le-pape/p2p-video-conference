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
 * @file netselectsocket.h
 * @brief Implementation of NetSelectReader on sockets
 */

#ifndef EPYX_NETSELECTSOCKET_H
#define EPYX_NETSELECTSOCKET_H

#include "netselectreader.h"
#include "socket.h"
#include <memory>

namespace Epyx
{
    /**
     * @class NetSelectSocket
     *
     * @brief NetSelect can watch wait data on sockets
     */
    class NetSelectSocket : public NetSelectReader
    {
    public:
        /**
         * @param sock socket shared pointer
         */
        NetSelectSocket(const std::shared_ptr<Socket>& sock);

        /**
         * @brief Constructor used with new Socket()
         * @param psock socket pointer
         */
        NetSelectSocket(Socket *psock);

        int getFileDescriptor() const;

        bool read();

    protected:
        /**
         * @brief Eat read data
         */
        virtual void eat(const byte_str& data) = 0;

        /**
         * @brief Get internal socket
         */
        const std::shared_ptr<Socket>& socket() const;

    private:
        std::shared_ptr<Socket> sock;
    };
}
#endif /* EPYX_NETSELECTREADER_H */
