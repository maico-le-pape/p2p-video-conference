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
 * @file actor-manager.cpp
 * @brief Implementation of the actor framework (non-templated part)
 */

#include "actor-manager.h"
#include "actor-id.h"
#include "actor.h"

namespace Epyx
{

    ActorManager::ActorManager(int num_workers, const std::string& name) :
    wp(num_workers, name, this), actorCount(0), timeouts(this, name){
        timeouts.start();
    }

    ActorManager::~ActorManager() {
        //TODO: find out why it was not called
        wp.stop();
        //TODO finish and delete everything
    }

    void ActorManager::kill(ActorId_base a) {
        actorsLock.lock();

        std::map<int, Actor*>::iterator it;
        it = actors.find(a.id);

        //If we find the actor we only set the alive flag and send it
        //a message, it will be killed the next time it is processed
        if (it != actors.end()) {
            Actor* actor = (*it).second;
            actor->alive = false;
            actorsLock.unlock();

            this->post(a.id, NULL);
        } else {
            actorsLock.unlock();
        }
    }

    void ActorManager::post(int id, std::function<void()> msg) {
        wp.post(new std::pair<int, std::function<void()>>(id, msg));
    }

    void ActorManager::postTimeout(int id, Timeout time, std::function<void()> msg) {
        this->timeouts.addTimeout(time, id, msg);
    }

    ActorManager::ActorWorkers::ActorWorkers(int num_workers, const std::string& name,
        ActorManager* m)
    :WorkerPool<std::pair<int, std::function<void()>>>(num_workers, name),
    manager(m) {
    }

    //pair->first is the id of the actor and ->second the message
    //if actor does not exist, do noting
    //if he is dead remove it and do nothing
    //if message = null the actor will call timeout()
    //else he will call treat(message)
    void ActorManager::ActorWorkers::treat(std::unique_ptr<std::pair<int, std::function<void()>>> msg){
        ActorManager* m = this->manager;

        m->actorsLock.lock();

        auto it = m->actors.find(msg->first);
        if (it != m->actors.end()) {
            Actor* actor = (*it).second;

            //KEEP IT IN THIS ORDER, see kill() for more info
            //there won't be deadlocks as we always acquire
            //actor before unlocking actorsLock
            //
            actor->lock();
            if (actor->alive) {
                m->actorsLock.unlock();
                actor->internal_treat(msg->second);
                actor->unlock();
            } else {
                actor->unlock();
                delete actor;
                m->actors.erase(it);
                m->actorsLock.unlock();
            }
        }else{
            m->actorsLock.unlock();
        }
    }

    bool ActorManager::ActorTimeoutComparator::operator()(const TimeoutEntry& a, const TimeoutEntry& b){
        return ! (a.time < b.time);
    }

    ActorManager::TimeoutLauncher::TimeoutLauncher(ActorManager* m, const std::string& name): Thread(name + "Timeouts", 0), manager(m) {
    }

    ActorManager::TimeoutLauncher::~TimeoutLauncher(){
        incoming.close();
        this->wait();
    }

    void ActorManager::TimeoutLauncher::addTimeout(Timeout t, int id, std::function<void()> method){
        incoming.push(new TimeoutEntry{t, id, method});
    }

    void ActorManager::TimeoutLauncher::run(){
        const int epsilon = 2;

        std::unique_ptr<TimeoutEntry> to_add = nullptr;
        do{
            //Wait for a new entry or until something times out
            if(timeouts.empty()){
                to_add = incoming.pop();
            }else{
                to_add = incoming.pop(timeouts.top().time.remainingMsec() + epsilon);
            }

            if(to_add){
                timeouts.push(*to_add);
                to_add.reset();
            }

            while(!timeouts.empty() && timeouts.top().time.hasExpired()){
                manager->post(timeouts.top().id, timeouts.top().method);
                timeouts.pop();
            }

        }while(incoming.isOpened());
    }
}

