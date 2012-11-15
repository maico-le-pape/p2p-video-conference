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
 * @file actor-manager.h
 * @brief Definition of the actor framework
 */

#ifndef EPYX_CORE_ACTOR_MANAGER_H
#define EPYX_CORE_ACTOR_MANAGER_H

#include <functional>
#include <mutex>
#include "worker-pool.h"
#include "timeout.h"
#include <atomic>
#include <string>
#include <map>
#include <vector>

namespace Epyx
{
    class Actor;
    struct ActorId_base;
    template<typename T> struct ActorId;
    /**
     * @class ActorManager
     *
     * @brief The framework that will run our actors
     *
     * The ActorManager takes care of running the actors, it's basic use
     * is to add an actor into it and then use the returned ActorId to
     * send messages or kill the actor. Actors can also be added with
     * a timeout.
     */
    class ActorManager
    {
    public:
        /**
         * @brief the ActorManager constructor
         * @param num_workers the number of threads to run the actors on
         * @param name the base name used in the logging system
         */
        ActorManager(int num_workers, const std::string& name);

        /**
         * @brief the ActorManager destructor
         */
        ~ActorManager();

        /**
         * @brief check is an Actor is still alive
         *
         * Note: the Actor may die just after the call, this method
         * only provide a hint.
         */
        void has(ActorId_base id);

        /**
         * @brief Adds an Actor (pointer no-timeout version)
         * @param a the actor to be added
         * @return An ActorId for this Actor
         *
         * Note: After this call you should only use the ActorId to
         * give commands to your actor.
         */
        template<typename T> ActorId<T> add(T* a);

        /**
         * @brief Adds an Actor (reference no-timeout version)
         * @param a the actor to be added
         * @return An ActorId for this Actor
         *
         * Note: After this call you should only use the ActorId to
         * give commands to your actor.
         */
        template<typename T> ActorId<T> add(T& a);

        /**
         * @brief Adds an Actor (pointer timeout version)
         * @param a the actor to be added
         * @param t int the time before the timeout in msec
         * @return An ActorId for this Actor
         *
         * Note: After this call you should only use the ActorId to
         * give commands to your actor.
         */
        template<typename T> ActorId<T> add(T* a, Timeout t);

        /**
         * @brief Adds an Actor (reference timeout version)
         * @param a the actor to be added
         * @param t int the time before the timeout in msec
         * @return An ActorId for this Actor
         *
         * Note: After this call you should only use the ActorId to
         * give commands to your actor.
         */
        template<typename T> ActorId<T> add(T& a, Timeout t);

        /**
         * @brief Kills an Actor contained in this manager, it won't receive anmore messges
         * @param id the ActorId of the Actor to be killed
         */
        void kill(ActorId_base id);

        void post(int id, std::function<void()> msg);
        void postTimeout(int id, Timeout time, std::function<void()> msg);

    private:
        class ActorWorkers : public WorkerPool<std::pair<int, std::function<void()>>>
        {
        public:

            ActorWorkers(int num_workers, const std::string& name, ActorManager* m);
            virtual void treat(std::unique_ptr<std::pair<int, std::function<void()>>> msg);

        private:
            ActorManager* manager;
        };

        ActorWorkers wp;
        std::map<int, Actor*> actors;
        std::mutex actorsLock;
        std::atomic<unsigned int> actorCount;

        //The thread used to fire timeouts
        struct TimeoutEntry
        {
            Timeout time;
            int id;
            std::function<void()> method;
        };

        struct ActorTimeoutComparator
        {
            bool operator()(const TimeoutEntry& a, const TimeoutEntry& b);
        };


        class TimeoutLauncher: public Thread
        {
        public:
            TimeoutLauncher(ActorManager* m, const std::string& name);
            ~TimeoutLauncher();
            void addTimeout(Timeout t, int id, std::function<void()> method);

        protected:
            virtual void run();

        private:
            ActorManager* manager;
            std::priority_queue<TimeoutEntry, std::vector<TimeoutEntry>, ActorTimeoutComparator> timeouts;
            BlockingQueue<TimeoutEntry> incoming;
        };

        TimeoutLauncher timeouts;
    };
}
#include "actor-manager-detail.h"

#endif /* EPYX_CORE_ACTOR_MANAGER_H */
