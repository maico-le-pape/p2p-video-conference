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
 * @file worker-pool.h
 * @brief a worker pool definition.
 */
#ifndef EPYX_WORKER_POOL_H
#define EPYX_WORKER_POOL_H

#include "blocking-queue.h"
#include <atomic>
#include <boost/noncopyable.hpp>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <sstream>
#include <thread>

namespace Epyx
{
    /**
     * @class WorkerPool
     *
     * @brief A mean to distribute work between several threads easily
     *
     * A WorkerPool allows you to distribute a work on several threads by
     * passing "messages" that are in facts function arguments. To use it
     * subclass the WorkerPool and implement the treat() function that will
     * process the message in a thread.
     *
     * @tparam T the base type of the messages passed to the workers
     */
    template<typename T>class WorkerPool : private boost::noncopyable
    {
    public:
        /**
         * @brief Internal shortcut for unique_ptr to type T
         */
        typedef std::unique_ptr<T> TPtr;

        /**
         * @brief The WorkerPool constructor
         * @param num_workers the initial number of worker threads
         * @param name the prefix of the name of the worker threads in the logs
         */
        WorkerPool(int num_workers, const std::string& name);

        /**
         * @brief The WorkerPool other constructor
         */
        WorkerPool();

        /**
         * @brief The WorkerPool destructor
         *
         * @todo What will happen with the messages that are still to be processed ?
         */
        virtual ~WorkerPool();

        /**
         * @brief Stop WorkerPool (wait all children threads)
         */
        void stop();

        /**
         * @brief Adds a message to be processed
         * @param message the message
         */
        void post(T* message);

        /**
         * @brief Adds a message to be processed
         * @param message the message
         */
        void post(TPtr message);

        /**
         * @brief set the name of the workers
         * Please call this functionto set a name BEFORE starting any worker
         */
        void setName(const std::string& name);

        /**
         * @brief gets the number of workers
         * @return the current number of workers
         */
        int getNumWorkers() const;

        /**
         * @brief sets the number of workers
         * @param n the number of workers needed
         */
        void setNumWorkers(int n);

    protected:
        /**
         * @brief this is the method to override
         * @param message the message to be processed
         */
        virtual void treat(TPtr message) = 0;

    private:
        void addWorker();
        void removeWorker();

        // Finalise a worker destruction
        void bookKeep();

        // Worker threads
        class Worker
        {
        public:
            // Spawn a new thread
            Worker(WorkerPool *pool, int id);

            // Tell the worker to stop ASAP, return the previous state
            bool tellStop();

            // Wait for the thread to terminate
            void wait();
        private:
            void run(WorkerPool *pool);

            bool running;
            int id;
            std::thread thread;
        };

        // Message queue
        BlockingQueue<T> messages;

        // Name prefix
        std::string name;

        // Last used ID, to be used for new workers
        std::atomic<int> lastId;

        // Running workers
        std::atomic<int> worker_count;
        std::mutex workers_mutex;
        std::map<int, std::unique_ptr<Worker> > workers;

        // Terminated workers, waiting a join()
        BlockingQueue<int> workers_to_destroy;
    };

}

#include "worker-pool-detail.h"

#endif /* EPYX_WORKER_POOL_H */
