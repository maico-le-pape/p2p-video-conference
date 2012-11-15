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
 * @file udpserver.h
 * @brief UDP server abstraction layer
 */
#ifndef EPYX_UDPSERVER_H
#define EPYX_UDPSERVER_H

#include "server.h"
#include "udpsocket.h"

namespace Epyx
{
    /**
     * @class UDPServer
     * @brief UDP server abstraction layer
     */
    class UDPServer : public Server
    {
    public:
        /**
         * @brief Create a new UDP server
         * @param addr IP address and UDP port to bind to
         */
        UDPServer(const SockAddress& addr);

        /**
         * @brief Override close to close only once the socket
         */
        virtual void close();

        /**
         * @brief Receive data for the server
         *
         * @param data received data buffer
         * @param size size of data buffer
         * @return number of received bytes
         */
        int recv(void *data, int size);

        // send is not implemented yet
        //int send(const void *data, int size);

        /**
         * @brief Bind socket to a specific device
         * @param devicename
         */
        void bindToDevice(const std::string& devicename);

    private:
        UDPSocket sock;
    };
}
#endif /* EPYX_UDPSERVER_H */

