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

#ifndef EPYX_CORE_ACTOR_ID_DETAIL_H
#define EPYX_CORE_ACTOR_ID_DETAIL_H

#include "actor-manager.h"

namespace Epyx
{

    template<typename T> ActorId<T>::ActorId(int id, ActorManager* manager, T* actor)
    :ActorId_base{id, manager}, actor(actor) {}

    template<typename T> ActorId<T>::ActorId(ActorManager* m, int i) {
        manager = m;
        id = i;
    }

    template<typename T> template <typename ... Args> void ActorId<T>::post(Args ... args) {
        std::function<void(T&, Args ...)> f = (void (T::*)(Args ...))&T::treat;
        manager->post(id, std::bind(f, std::ref(*actor), args ...));
    }

    template<typename T> template <typename ... Args> void ActorId<T>::timeout(Timeout time, Args ... args) {
        std::function<void(T&, Args ...)> f = (void (T::*)(Args ...))&T::timeout;
        manager->postTimeout(id, time, std::bind(f, std::ref(*actor), args ...));
    }

    template<typename T> void ActorId<T>::kill() {
        manager->kill(*this);
    }
}
#endif //EPYX_CORE_ACTOR_ID_DETAIL_H
