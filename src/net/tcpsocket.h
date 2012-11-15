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
 * @file tcpsocket.h
 * @brief TCP socket abstraction layer
 */
#ifndef EPYX_TCPSOCKET_H
#define EPYX_TCPSOCKET_H

#include "socket.h"

namespace Epyx
{

    /**
     * @class TCPSocket
     *
     * @brief TCP socket abstraction layer
     */
    class TCPSocket : public Socket
    {
    public:
        /**
         * @brief Initialise an empty TCP socket
         */
        TCPSocket();
        /**
         * @brief Initialise a TCP socket with a remote address
         */
        TCPSocket(const SockAddress& addr);
        /**
         * @brief Build a new Socket object from an existing connection
         */
        TCPSocket(int sock, const SockAddress &addr);

        /**
         * @brief Send data through the socket
         *
         * @param data sent data
         * @param size size of data to be sent
         * @return number of sent bytes
         */
        int send(const void *data, int size);

        /**
         * @brief Receive data from the socket
         *
         * @param data received data buffer
         * @param size size of data buffer
         * @return number of received bytes
         */
        int recv(void *data, int size);
    private:
        bool connect();
        bool isConnected;
    };

}
#endif /* EPYX_UDPSOCKET_H */
