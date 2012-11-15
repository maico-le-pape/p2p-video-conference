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
 * @file thread.h
 * @brief Thread management astraction layer
 */
#ifndef EPYX_THREAD_H
#define EPYX_THREAD_H

#include <boost/noncopyable.hpp>
#include <string>
#include <pthread.h>
#include "tls-pointer.h"

namespace Epyx
{
    namespace detail
    {
        /**
         * @private thread_infos
         */
        extern TLSPointer<std::string>* thread_name;

        /**
         * @private thread_initialized
         */
        extern bool thread_initialized;
    }
    /**
     * @class Thread
     *
     * @brief Thread management system
     *
     * To define a thread, you should inherit from this abstract class.
     */
    class Thread : private boost::noncopyable
    {
    public:

        /**
         * @brief Thread constructor
         */
        Thread();

        /**
         * @brief Give a name in the constructor
         * @param name given name
         * @param id Thread ID
         */
        Thread(const std::string& name, int id = -1);

        /**
         * @brief Basic destructor, which terminates the thread
         */
        virtual ~Thread();

        /**
         * @brief Set name-id if thread was created without one DEPRECATED
         * @param name given name
         * @param id Thread ID
         */
        void setThreadName(const std::string& name, int id = -1);

        /**
         * @brief Start the thread
         */
        bool start();

        /**
         * @brief Wait until the end
         */
        void wait();

        /**
         * @brief Terminate a thread
         */
        void term();

        /**
         * @brief Get the name of this thread
         * @return this->name
         */
        const std::string& getThisName();

        /**
         * @brief Set the name of the running thread
         * @param name the name of the thread
         */
        static void setName(const std::string& name);


        /**
         * @brief Get the name of the running thread
         * @return [current thread]->name, not this->name
         */
        static std::string getName();

        /**
         * @brief Intialise the thread system
         * @param name Name of the main thread
         * @param id ID of the main thread
         */
        static void init(const std::string& name = "Main", int id = -1);
        /**
         * @brief Tell wether the thread system is initialised
         * @return true if Thread::init() was called
         */
        static bool isInitialized();

    protected:
        /**
         * @brief Thread entry point
         */
        virtual void run() = 0;

    private:
        /**
         * @brief Thread entry point
         * @param Thread object
         * @return NULL
         */
        static void* _thread_start(void*);

        //And this is the ofstream of each stream
        pthread_t thread;
        std::string name;
    };
}

#endif /* EPYX_THREAD_H */
