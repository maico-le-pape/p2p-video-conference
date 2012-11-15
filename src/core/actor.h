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
 * @file actor.h
 * @brief Definition of the actor class that will need to be extended to mae your own actors
 */

#ifndef EPYX_CORE_ACTOR_H
#define EPYX_CORE_ACTOR_H

#include <mutex>
#include "actor-id.h"
#include "actor-manager.h"
#include "name-struct.h"
#include "common.h"
namespace Epyx
{
    class ActorManager;

    /**
     * @class Actor
     *
     * @brief A way to implement you own actors
     *
     * Basically an actor can do only 2 things: receiving messages and timing out.
     * To do this overrride the two methods treat and timeout.
     *
     * @tparam T the base type of the messages the actor can receive
     */
    class Actor
    {
    public:
        virtual ~Actor();

        void setId(ActorId_base id);
        ActorId_base getBaseId();

        /**
         * @brief locks this actor
         *
         * DO NOT USE INSIDE treat() OR timeout()
         */
        void lock();

        /**
         * @brief unlocks this actor
         */
        void unlock();

        bool alive;

        void internal_treat(std::function<void()> msg);

        template<typename T>static ActorId<T> getId(T* actor) {
            return {actor->self.id, actor->self.manager, actor};
        }

    protected:
        /**
         * @brief kills the actor (it won't timeout or receive other messages)
         */
        void kill();

    private:
        std::mutex mutex;
        ActorId_base self;

    };

    #define EPYX_ACTOR_QUERY(str1) EPYX_NAME_STRUCT_INSTANCE(str1)
    #define EPYX_ACTOR_QUERY_ARG(str1) EPYX_NAME_STRUCT(str1)

    //Shorcut
    #define EPYX_AQ(s) EPYX_ACTOR_QUERY(s)
    #define EPYX_AQA(s) EPYX_ACTOR_QUERY_ARG(s)
}
#endif //EPYX_CORE_ACTOR_H
