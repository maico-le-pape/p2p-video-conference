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
 * @file netquery.h
 * @brief Use sockets for a single query
 */

#ifndef EPYX_NETQUERY_H
#define EPYX_NETQUERY_H

#include "socket.h"
#include "../core/timeout.h"
#include <boost/noncopyable.hpp>
#include <memory>

namespace Epyx
{
    /**
     * @class NetQuery
     * @brief Manage a socket which is used only once
     *
     * Usage:
     *  1- Instantiate a NetQuery object
     *  2- Set socket (UDP or TCP, which has an address)
     *  3- Send query
     *  4- Wait for receiving answer (with a timeout)
     *  5- Close everything
     *
     * Methods:
     *  1- Constructor
     *  2- setSocket(sock) or constructor
     *  3- abstract query() method which calls socket().send()
     *  4- T* answer = answer(timeout) which calls abstract eat(data) on
     *     each received data.
     *  5- Destructor or close()
     */
    template<typename T>class NetQuery : private boost::noncopyable
    {
    public:
        /**
         * @brief Empty constructor
         */
        NetQuery();

        /**
         * @brief Constructor with a socket
         * @param sock socket shared pointer
         */
        NetQuery(const std::shared_ptr<Socket>& sock);

        /**
         * @brief Constructor with a newsocket
         * @param sock new socket
         */
        NetQuery(Socket* sock);

        /**
         * @brief Desctructor
         */
        virtual ~NetQuery();

        /**
         * @brief Set socket
         * @param sock
         */
        void setSocket(const std::shared_ptr<Socket>& sock);

        /**
         * @brief Close everything
         */
        void close();

        /**
         * @brief send query
         * @return false if there is a problem
         */
        virtual bool query() = 0;

        /**
         * @brief Wait for an answer
         * @param timeout maximum number of seconds to wait for an answer
         * @return the answer, or NULL if there is a timeout or an error
         */
        std::unique_ptr<T> answer(const Timeout& timeout);

        /**
         * @brief Same as answer(), but modify the given reference
         * @param timeout
         * @param ans pointer wher answer is copied
         * @return true on success
         */
        bool answerIn(const Timeout& timeout, T *ans);

        /**
         * @brief query() and answer() in one call
         * @param timeout
         * @return answer
         */
        std::unique_ptr<T> queryAnswer(const Timeout& timeout);
        /**
         * @brief query() and answerIn() in one call
         * @param timeout
         * @param ans
         * @return answer
         */
        bool queryAnswerIn(const Timeout& timeout, T *ans);

    protected:
        /**
         * @brief Get internal socket
         * @return sock
         */
        const std::shared_ptr<Socket>& socket() const;

        /**
         * @brief Eat data and return an answer once it is ready
         * @param data data to eat
         * @param size available size
         * @return NULL if data is incomplete, the newly-allocated answer otherwise
         */
        virtual std::unique_ptr<T> eat(const byte_str& data) = 0;

    private:
        std::shared_ptr<Socket> sock;
    };
}

#include "netquery-detail.h"

#endif /* EPYX_NETQUERY_H */
