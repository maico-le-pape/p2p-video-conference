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
#include "../core/common.h"
#include <sys/select.h>
#include <sys/time.h>

namespace Epyx
{

    template<typename T> NetQuery<T>::NetQuery()
    : sock(NULL) {
    }

    template<typename T> NetQuery<T>::NetQuery(const std::shared_ptr<Socket>& sock)
    : sock(sock) {
    }

    template<typename T> NetQuery<T>::NetQuery(Socket* sock)
    : sock(sock) {
    }

    template<typename T> NetQuery<T>::~NetQuery() {
        this->close();
    }

    template<typename T> void NetQuery<T>::setSocket(const std::shared_ptr<Socket>& sock) {
        EPYX_ASSERT(!this->sock && sock);
        this->sock = sock;
    }

    template<typename T> void NetQuery<T>::close() {
        if (sock) {
            sock->close();
            sock.reset();
        }
    }

    template<typename T> std::unique_ptr<T> NetQuery<T>::answer(const Timeout& timeout) {
        struct timeval tv;
        fd_set rfds;
        const int size = 4096;
        byte data[size];
        std::unique_ptr<T> ans;

        EPYX_ASSERT(sock != NULL);
        int sockfd = sock->getFd();
        if (sockfd < 0)
            return ans;

        // select() loop
        while (!timeout.hasExpired()) {
            // Wait with select()
            FD_ZERO(&rfds);
            FD_SET(sockfd, &rfds);
            tv = timeout.remainingTimeval();
            int status = select(sockfd + 1, &rfds, NULL, NULL, &tv);
            if (status < -1)
                throw ErrException("NetSelect", "select");

            // Read data
            if (FD_ISSET(sockfd, &rfds)) {
                int recvSize = sock->recv(data, size);

                try {
                    // Eat data and return any answer
                    // Note: if recvSize = 0, this is the last call to eat()
                    ans = this->eat(byte_str(data, recvSize));
                    if (ans)
                        return ans;

                    // Close on End-Of-File
                    if (recvSize == 0) {
                        this->close();
                        return ans;
                    }
                } catch (Exception e) {
                    log::error << e << log::endl;
                    log::error << "Closing socket to " << sock->getAddress() <<
                            " due to an exception" << log::endl;
                    this->close();
                    return ans;
                }
            }
        }
        return ans;
    }

    template<typename T> bool NetQuery<T>::answerIn(const Timeout& timeout, T *ans) {
        std::unique_ptr<T> result = this->answer(timeout);
        if (!result)
            return false;
        if (ans != NULL)
            *ans = *result;
        return true;
    }

    template<typename T> std::unique_ptr<T> NetQuery<T>::queryAnswer(const Timeout& timeout) {
        if (!this->query()) {
            log::error << "NetQuery: Unable to send query" << log::endl;
            return NULL;
        }
        return this->answer(timeout);
    }

    template<typename T> bool NetQuery<T>::queryAnswerIn(const Timeout& timeout, T *ans) {
        if (!this->query()) {
            log::error << "NetQuery: Unable to send query" << log::endl;
            return false;
        }
        return this->answerIn(timeout, ans);
    }

    template<typename T> const std::shared_ptr<Socket>& NetQuery<T>::socket() const {
        EPYX_ASSERT(sock);
        return sock;
    }
}
