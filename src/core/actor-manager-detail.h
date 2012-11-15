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
 * @file actor-manager-detail.h
 * @brief Implementation of the actor framework (templated part)
 */

#ifndef EPYX_CORE_ACTOR_MANAGER_DETAIL_H
#define EPYX_CORE_ACTOR_MANAGER_DETAIL_H

namespace Epyx
{
    template<typename T> ActorId<T> ActorManager::add(T* a) {
        EPYX_ASSERT(a != NULL);
        return this->add(*a);
    }

    template<typename T> ActorId<T> ActorManager::add(T& a) {
        unsigned int i = std::atomic_fetch_add(&actorCount, 1U);
        ActorId<T> res(i, this, &a);
        a.setId(res);

        a.alive = true;

        {
            std::lock_guard<std::mutex> lock(actorsLock);
            actors[i] = &a;
        }

        return res;
    }

    template<typename T> ActorId<T> ActorManager::add(T* a, Timeout t) {
        EPYX_ASSERT(a != NULL);
        return this->add(*a, t);
    }

    template<typename T> ActorId<T> ActorManager::add(T& a, Timeout t) {
        ActorId<T> res = this->add(a);
        res.timeout(t);
        return res;
    }
}

#endif /* EPYX_CORE_ACTOR_MANAGER_DETAIL_H */
