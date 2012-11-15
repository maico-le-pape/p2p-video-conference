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
 * @file worker-pool-detail.h
 * @brief a worker pool implementation
 *
 * You should never include this file directly
 */
#ifndef EPYX_WORKER_POOL_DETAIL_H
#define EPYX_WORKER_POOL_DETAIL_H

#include "log.h"

namespace Epyx
{

    template<typename T> WorkerPool<T>::WorkerPool(int num_workers, const std::string& name)
    : name(name), lastId(0), worker_count(0) {
        for (int i = 0; i < num_workers; i++) {
            this->addWorker();
        }
    }

    template<typename T> WorkerPool<T>::WorkerPool()
    : name(""), lastId(0), worker_count(0) {
    }

    template<typename T> WorkerPool<T>::~WorkerPool() {
        this->stop();
    }

    template<typename T> void WorkerPool<T>::stop() {
        // Close message queue
        this->messages.close();

        // Remove all workers
        {
            std::lock_guard<std::mutex> lock(workers_mutex);
            for (auto it = workers.begin(); it != workers.end(); ++it) {
                it->second->tellStop();
            }
        }

        while (worker_count > 0) {
            // Wait all workers
            std::unique_ptr<int> id = workers_to_destroy.pop();
            std::lock_guard<std::mutex> lock(workers_mutex);
            auto it = workers.find(*id);
            if (it != workers.end()) {
                it->second->wait();
                it->second.reset();
                workers.erase(it);
                std::atomic_fetch_sub(&worker_count, 1);
            }
        }
    }

    template<typename T> void WorkerPool<T>::post(T* message) {
        EPYX_ASSERT(message != NULL);
        this->post(TPtr(message));
    }

    template<typename T> void WorkerPool<T>::post(TPtr message) {
        this->bookKeep();
        this->messages.push(message);
    }

    template<typename T> void WorkerPool<T>::setName(const std::string& name) {
        EPYX_ASSERT(this->name.empty());
        this->name = name;
    }

    template<typename T> int WorkerPool<T>::getNumWorkers() const {
        return this->worker_count;
    }

    template<typename T> void WorkerPool<T>::setNumWorkers(int n) {
        EPYX_ASSERT(n >= 0);
        if (this->worker_count > n) {
            int to_remove = this->worker_count - n;
            for (int i = 0; i < to_remove; i++) {
                this->removeWorker();
            }
        } else {
            int to_add = n - this->worker_count;
            for (int i = 0; i < to_add; i++) {
                this->addWorker();
            }
        }
    }

    template<typename T> void WorkerPool<T>::addWorker() {
        std::lock_guard<std::mutex> lock(workers_mutex);
        int id = std::atomic_fetch_add(&lastId, 1);
        workers[id] = std::unique_ptr<Worker > (new Worker(this, id));
        std::atomic_fetch_add(&worker_count, 1);
    }

    template<typename T> void WorkerPool<T>::removeWorker() {
        std::lock_guard<std::mutex> lock(workers_mutex);
        // Find first running worker
        for (auto it = workers.begin(); it != workers.end(); ++it) {
            if (it->second->tellStop()) {
                // "Tell stop" has really done something
                return;
            }
        }
    }

    template<typename T> void WorkerPool<T>::bookKeep() {
        // Find a terminated worker
        std::unique_ptr<Worker> worker;
        std::unique_ptr<int> id = workers_to_destroy.tryPop();
        if (id) {
            std::lock_guard<std::mutex> lock(workers_mutex);
            auto it = workers.find(*id);
            if (it != workers.end()) {
                it->second.swap(worker);
                workers.erase(it);
            }
        }
        if (worker) {
            // worker->running is already set to false, let's join it
            worker->wait();
            std::atomic_fetch_sub(&worker_count, 1);
        }
    }

    template<typename T> WorkerPool<T>::Worker::Worker(WorkerPool<T>* pool, int id)
    : running(true), id(id), thread(&WorkerPool<T>::Worker::run, this, pool) {
    }

    template<typename T> void WorkerPool<T>::Worker::run(WorkerPool *pool) {
        EPYX_ASSERT(pool != NULL);
        std::ostringstream str;
        str << pool->name << " " << id;
        Thread::setName(str.str());
        try {
            while (running) {
                TPtr msg = pool->messages.pop();
                if (msg) {
                    pool->treat(std::move(msg));
                }
            }
        } catch (std::exception& e) {
            // Log message and crash
            log::fatal << "[Exception] Unhandled exception: " << e.what() << log::endl;
            log::waitFlush();
            throw e;
        }
        pool->workers_to_destroy.push(new int(id));
    }

    template<typename T> bool WorkerPool<T>::Worker::tellStop() {
        bool oldState = running;
        running = false;
        return oldState;
    }

    template<typename T> void WorkerPool<T>::Worker::wait() {
        // Waiting for the thread to finish
        EPYX_ASSERT(!running);
        thread.join();
    }
}

#endif /* EPYX_WORKER_POOL_DETAIL_H */
