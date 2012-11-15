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
 * @file netselectreader.h
 * @brief object on which select () is ran
 */

#ifndef EPYX_NETSELECTREADER_H
#define EPYX_NETSELECTREADER_H

#include <boost/noncopyable.hpp>

namespace Epyx
{
    class NetSelect;
    /**
     * @class NetSelectReader
     *
     * @brief Container used by NetSelect to remember watched sockets
     */
    class NetSelectReader : private boost::noncopyable
    {
    public:
        NetSelectReader();
        virtual ~NetSelectReader();

        /**
         * @brief Get the file descriptor of the socket which is select()'ed
         * @return socket_fd, or -1 if it is closed
         */
        virtual int getFileDescriptor() const = 0;

        /**
         * @brief Read everything it can read and returns
         * @return true on success, false on error or end of file
         */
        virtual bool read() = 0;

        /**
         * @brief Close the read channel
         */
        void close();

        /**
         * @brief Get the NetSelect objects which owns this object
         */
        NetSelect* getOwner() const;

        /**
         * @brief Set owner, only once
         */
        void setOwner(NetSelect *ns);
    private:
        // NetSelect which owns this reader
        NetSelect *owner;
    };
}
#endif /* EPYX_NETSELECTREADER_H */
