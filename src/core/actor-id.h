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
 * @file actor-id.h
 */

#ifndef EPYX_CORE_ACTOR_ID_H
#define EPYX_CORE_ACTOR_ID_H

#include "timeout.h"

namespace Epyx
{
    class ActorManager;

    /**
     * @struct ActorId_base
     * @brief the base class of the ActorId, do not use directly
     */
    struct ActorId_base
    {
        int id;
        ActorManager* manager;
    };
    /**
     * @struct ActorId
     * @brief Identifies and sends commands to an Actor
     * @tparam T the template parameter of the corresponding actor
     */
    template<typename T> struct ActorId : public ActorId_base
    {
        /**
         * @brief ActorId's default constructor
         */
        ActorId(int id, ActorManager* manager, T* actor);

        /**
         * @brief ActorId's other constructor
         */
        ActorId(ActorManager* m, int i);

        /**
         * @brief sends a message to its Actor
         * @param msg the message
         */
        template<typename ... Args> void post(Args ... args);

        /**
         * @brief sends a message to its Actor after a certain time
         * @param length the length of the timeout
         * @param msg the message
         */
        template<typename ... Args> void timeout(Timeout time, Args ... args);

        /**
         * @brief kills its Actor
         */
        void kill();
    private:
        T* actor;
    };
}

#include "actor-id-detail.h"

#endif //EPYX_CORE_ACTOR_ID_H
