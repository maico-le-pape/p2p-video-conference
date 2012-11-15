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
 * @file socket.h
 * @brief Socket astraction layer
 */

#ifndef EPYX_SOCKET_H
#define EPYX_SOCKET_H

#include "sockaddress.h"
#include "../core/byte.h"
#include <boost/noncopyable.hpp>
#include <string>

namespace Epyx
{
    /**
     * @class Socket
     *
     * @brief Socket abstraction layer
     */
    class Socket : private boost::noncopyable
    {
    public:
        /**
         * @brief Global initialisation of Socket API
         */
        static void init();

        /**
         * @brief Global finition of Socket API
         */
        static void fini();

        /**
         * @brief Initialise an empty socket
         */
        Socket();
        /**
         * @brief Initialise a socket with a remote address
         */
        Socket(const SockAddress& addr);
        /**
         * @brief Build a new Socket object from an existing connection
         */
        Socket(int sock, const SockAddress &addr);
        /**
         * @brief Destructor
         */
        virtual ~Socket();

        /**
         * @brief Set a new file descriptor
         *
         * NOTE: The socket must be closed before calling this method.
         */
        void setFd(int sockfd);
        /**
         * @brief Get the internal file descriptor, or -1 if it is closed
         */
        int getFd() const;
        /**
         * @brief Tell wether the socket is opened or closed
         */
        bool isOpened() const;

        /**
         * @brief Set the remote address
         */
        void setAddress(const SockAddress& addr);
        /**
         * @brief Get the remote address
         */
        SockAddress getAddress() const;
        /**
         * @brief Set the local address
         */
        void setLocalAddress(const SockAddress& addr);
        /**
         * @brief Get the local address
         */
        SockAddress getLocalAddress() const;
        /**
         * @brief Update local address from socket
         */
        void updateLocalAddress();

        /**
         * @brief Shutdown and close the socket
         */
        void close();

        /**
         * @brief Virtual method to send data
         * @param data buffer pointer
         * @param size buffer size
         * @return number of sent bytes
         */
        virtual int send(const void *data, int size) = 0;
        /**
         * @brief Send all bytes through the network
         * @param data buffer pointer
         * @param size buffer size
         * @return true if all data was sent
         */
        bool sendAll(const void *data, int size);
        /**
         * @brief Send all bytes through the network
         * @param bytes
         * @return true if all data was sent
         */
        bool sendBytes(const byte_str& bytes);
        /**
         * @brief Send text through the network
         * @param message text to send
         * @return true on success
         */
        bool write(const std::string& message);

        /**
         * @brief Virtual method to receive data
         * @param data buffer pointer
         * @param size buffer size
         * @return number of received bytes
         */
        virtual int recv(void *data, int size) = 0;
        /**
         * @brief Receive exactly (size) bytes from the network
         * @param data buffer pointer
         * @param size buffer size
         * @return true if the buffer is filled
         */
        bool recvAll(void *data, int size);
        /**
         * @brief Receive exactly (size) bytes from the network
         * @param data byte_str pointer
         * @param size buffer size
         * @return true if the buffer is filled
         */
        bool recvBytes(byte_str *data, int size);

    protected:
        /**
         * @brief Socket file descriptor
         */
        int sock;

        /**
         * @brief IP + port associated with the socket
         */
        SockAddress address;

        /**
         * @brief Local address for the socket or "*" if unknown
         */
        SockAddress localAddress;

    private:
        // Global initialisation stuff
        static bool is_init;

        // Last end-of-line caracter (\r or \n)
        char last_eol;
    };
}
#endif /* EPYX_SOCKET_H */
