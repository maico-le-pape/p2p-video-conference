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
 * @file udpsocket.h
 * @brief UDP socket abstraction layer
 */
#ifndef EPYX_UDPSOCKET_H
#define EPYX_UDPSOCKET_H

#include "socket.h"

namespace Epyx
{
    class UDPServer;

    /**
     * @class UDPSocket
     * @brief UDP socket abstraction layer
     */
    class UDPSocket : public Socket
    {
    public:
        /**
         * @brief Initialise an empty UDP socket
         */
        UDPSocket();
        /**
         * @brief Initialise an UDP socket with a remote address
         */
        UDPSocket(const SockAddress& addr, bool isBroadcast = false);
        /**
         * @brief Build a new Socket object from an existing connection
         */
        UDPSocket(int sock, const SockAddress &addr);

        /**
         * @brief Initialise the internal socket
         * @throws ErrException if socket() fails
         */
        void create();

        /**
         * @brief Enable or disable broadcast
         * @param enable
         */
        void setBroadcast(bool enable);

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

        /**
         * @brief Get the remote address from which the last packet was received
         * @return remote address
         */
        SockAddress getLastRecvAddr() const;

        /**
         * @brief Bind socket to a specific device
         * @param devicename
         */
        void bindToDevice(const std::string& devicename);

    private:
        SockAddress lastRecvAddr;
    };

}
#endif /* EPYX_UDPSOCKET_H */
