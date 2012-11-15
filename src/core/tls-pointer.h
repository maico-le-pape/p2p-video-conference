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
 * @file tls-pointer.h
 * @brief TLS pointer definition.
 */
#ifndef EPYX_TLS_POINTER_H
#define EPYX_TLS_POINTER_H

#include <boost/noncopyable.hpp>
#include <pthread.h>
#include "assert.h"
#include "exception.h"


namespace Epyx
{
    /**
     * @class TLSPointer
     *
     * @brief A pointer with a per-thread value
     *
     * Thread Local Storage is used when you want a variable that is local to
     * a thread : it acts as a global variable that is different in each thread.
     * The optional constructor argument is a function that will be used to
     * populate the pointer when new threads are spawned. The destructor is
     * called only whe the thread is terminated from the outside : when you
     * replace the pointer or when you end the thread normally, the estructor
     * is not called.
     *
     * @tparam T the base type of the pointers contained in the TLSPointer
     */
    template<typename T> class TLSPointer : private boost::noncopyable
    {
    public:
        /**
         * @brief A TLSPointer constructor
         * @param destructor (optional) a pointer to a destructor function
         */
        TLSPointer(void (*destructor)(T*) = &_default_destructor);

        /**
         * @brief A TLSPointer constructor
         * @param constructor a pointer to a constructor function
         * @param destructor (optional) a pointer to a destructor function
         */
        TLSPointer(T* (*constructor)(), void (*destructor)(T*) = &_default_destructor);

        /**
         * @brief the TLSPointer destructor
         */
        ~TLSPointer();

        /**
         * @brief get the per thread value of the pointer
         * @return the pointer value
         */
        T* get();

        /**
         * @brief sets the value of the pointer
         * @param new_value (optional) the new value of the pointer, null if omitted
         */
        void reset(T* new_value = NULL);

        /**
         * @brief equivalent to {temp = tls.get(); tls.reset(); return temp}
         * @return the value of the pointer
         */
        T* release();

    private:
        void (*destructor)(T*);
        T* (*constructor)();
        pthread_key_t key;

        // Destructor type with void*
        typedef void(void_destructor_func) (void*);

        static void _default_destructor(T* object);
    };

}

#include "tls-pointer-detail.h"

#endif /* EPYX_TLS_POINTER_H */
