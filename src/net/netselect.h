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
 * @file netselect.h
 * @brief Implement select() class to wait data on multiple net sockets
 */

#ifndef EPYX_NETSELECT_H
#define EPYX_NETSELECT_H

#include <atomic>
#include <memory>
#include <mutex>
#include "../core/worker-pool.h"
#include "netselectreader.h"

namespace Epyx
{
    class NetSelect;
    /**
     * @class NetSelect
     *
     * @brief Implement select() on multiple sockets using a WorkerPool
     *
     * Use NetSelect.start() to start this thread
     */
    class NetSelect : public Thread
    {
    public:
        /**
         * @brief Constructor
         * @param numworkers Initial workers number
         * @param workerName Name of workers
         */
        NetSelect(int numworkers, const std::string& workerName);
        /**
         * @brief Destructor
         */
        ~NetSelect();

        /**
         * @brief add a new reader
         * @param nsr shared pointer
         */
        int add(const std::shared_ptr<NetSelectReader>& nsr);

        /**
         * @brief Kill a reader
         * @param id
         */
        void kill(int id);

        /**
         * @brief Get a reader
         * @param id
         * @return the reader, or NULL if it does not exist
         */
        std::shared_ptr<NetSelectReader> get(int id);

        /**
         * @brief Get the number of workers
         * @return the current number of workers
         */
        int getNumWorkers() const;

        /**
         * @brief Set the number of workers
         * @param n the number of workers needed
         */
        void setNumWorkers(int n);

    protected:
        void run();

    private:
        /**
         * @brief NetSelectReader information
         */
        struct NetSelectReaderInfo
        {
            // Reader, which is shared to avoid lock between workers
            std::shared_ptr<NetSelectReader> reader;

            // It is alive
            bool alive;

            // It has data to be read (it is in the blocking queue)
            bool inQueue;

            NetSelectReaderInfo(const std::shared_ptr<NetSelectReader>& nsr);
        };

        std::atomic<int> lastId;
        std::mutex readersMutex;
        std::map<int, std::unique_ptr<NetSelectReaderInfo> > readers;

        // Workers class
        class Workers : public WorkerPool<int>
        {
        public:
            Workers(NetSelect *owner);
            void treat(std::unique_ptr<int> nsriId);
        private:
            NetSelect *owner;
        } workers;

        friend void Workers::treat(std::unique_ptr<int> nsriId);

        bool running;
    };
}
#endif /* EPYX_NETSELECT_H */
