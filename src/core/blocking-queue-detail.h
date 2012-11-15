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
 * @file blocking-queue-detail.h
 * @brief a blocking queue implementation
 *
 * You should never include this file directly
 */
#ifndef EPYX_BLOCKING_QUEUE_DETAIL_H
#define EPYX_BLOCKING_QUEUE_DETAIL_H

namespace Epyx
{

    template<typename T> BlockingQueue<T>::BlockingQueue()
    : fifo(new TQueue()), opened(true) {
    }

    template<typename T> BlockingQueue<T>::~BlockingQueue() {
        this->close();
    }

    template<typename T> void BlockingQueue<T>::close() {
        opened = false;
        cond.notify_all();
    }

    template<typename T> bool BlockingQueue<T>::isOpened() {
        return opened;
    }

    template<typename T> bool BlockingQueue<T>::push(T *e) {
        TPtr pe(e);
        return push(pe);
    }

    template<typename T> bool BlockingQueue<T>::push(TPtr& e) {
        if (!opened) {
            return false;
        }
        {
            std::lock_guard<std::mutex> lock(mut);
            fifo->push_back(std::move(e));
        }
        cond.notify_one();
        return true;
    }

    template<typename T> bool BlockingQueue<T>::tryPush(TPtr e) {
        if (opened && mut.try_lock()) {
            fifo->push_back(e);
            mut.unlock();
            cond.notify_one();
            return true;
        }
        return false;
    }

    template<typename T> typename BlockingQueue<T>::TPtr BlockingQueue<T>::pop() {
        std::unique_lock<std::mutex> lock(mut);
        while (opened && fifo->empty()) {
            cond.wait(lock);
        }
        if (fifo->empty()) {
            return TPtr();
        }

        // Continue to pop even if queue is closed
        TPtr result;
        fifo->front().swap(result);
        fifo->pop_front();
        return result;
    }

    template<typename T> typename BlockingQueue<T>::TPtr BlockingQueue<T>::pop(int msec) {
        std::unique_lock<std::mutex> lock(mut);
        if (opened && fifo->empty()) {
            if (cond.wait_for(lock, std::chrono::milliseconds(msec)) == std::cv_status::timeout) {
                // Timeout
                return nullptr;
            }
        }
        // There is nothing to return
        if (fifo->empty()) {
            return nullptr;
        }

        // Continue to pop even if queue is closed
        TPtr result;
        fifo->front().swap(result);
        fifo->pop_front();
        return result;
    }

    template<typename T> typename BlockingQueue<T>::TPtr BlockingQueue<T>::tryPop() {
        TPtr result = NULL;
        if (mut.try_lock()) {
            if (!fifo->empty()) {
                fifo->front().swap(result);
                fifo->pop_front();
            }
            mut.unlock();
        }
        return result;
    }

    template<typename T> typename BlockingQueue<T>::TQueuePtr BlockingQueue<T>::flush() {
        TQueuePtr result;
        std::unique_lock<std::mutex> lock(mut);
        while (opened && fifo->empty()) {
            cond.wait(lock);
        }
        if (!fifo->empty()) {
            result.swap(fifo);
            fifo.reset(new TQueue());
        }
        return result;
    }

    template<typename T> typename BlockingQueue<T>::TQueuePtr BlockingQueue<T>::flush(int msec) {
        TQueuePtr result;
        std::unique_lock<std::mutex> lock(mut);
        while (opened && fifo->empty()) {
            if (cond.wait_for(lock, std::chrono::milliseconds(msec)) == std::cv_status::timeout) {
                // Timeout
                return result;
            }
        }
        if (!fifo->empty()) {
            result.swap(fifo);
            fifo.reset(new TQueue());
        }
        return result;
    }

    template<typename T> typename BlockingQueue<T>::TQueuePtr BlockingQueue<T>::tryFlush() {
        TQueuePtr result;
        if (mut.try_lock()) {
            if (!fifo->empty()) {
                result.swap(fifo);
                fifo.reset(new TQueue());
            }
            mut.unlock();
        }
        return result;
    }

    template<typename T > size_t BlockingQueue<T>::size() {
        std::unique_lock<std::mutex> lock(mut);
        return fifo->size();
    }

    template<typename T> bool BlockingQueue<T>::empty() {
        std::unique_lock<std::mutex> lock(mut);
        return fifo->empty();
    }
}

#endif /* EPYX_BLOCKING_QUEUE_DETAIL_H */
