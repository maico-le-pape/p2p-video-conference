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
 * @file actor.cpp
 */

#include "actor.h"

namespace Epyx
{
    void Actor::lock() {
        this->mutex.lock();
    }

    void Actor::unlock() {
        this->mutex.unlock();
    }

    void Actor::setId(ActorId_base id) {
        self = id;
    }

    ActorId_base Actor::getBaseId() {
        return self;
    }

    Actor::~Actor() {}

    void Actor::internal_treat(std::function<void()> msg) {
        msg();
    }

    void Actor::kill() {
        this->alive = false;
        this->self.manager->post(this->self.id, nullptr);//Send a message to make sure the actor is destroyed
    }
}

