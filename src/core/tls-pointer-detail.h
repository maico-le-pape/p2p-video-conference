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
 * @file tls-pointer-detail.h
 * @brief TLS pointer implementation
 *
 * You should never include this file directly
 */
#ifndef EPYX_TLS_POINTER_DETAIL_H
#define EPYX_TLS_POINTER_DETAIL_H

#include "tls-pointer.h"


namespace Epyx
{

    template<typename T> void TLSPointer<T>::_default_destructor(T* object) {
        delete object;
    }

    template<typename T> TLSPointer<T>::TLSPointer(void (*destructor)(T*))
    : destructor(destructor), constructor(NULL) {
        int key_create_status = pthread_key_create(&key,
                reinterpret_cast<void_destructor_func*> (this->destructor));
        EPYX_ASSERT_NO_LOG(key_create_status == 0);
    }

    template<typename T> TLSPointer<T>::TLSPointer(T* (*constructor_)(), void (*destructor_)(T*))
    : destructor(destructor_), constructor(constructor_) {
        int key_create_status = pthread_key_create(&key,
                reinterpret_cast<void_destructor_func*> (this->destructor));
        EPYX_ASSERT_NO_LOG(key_create_status == 0);
    }

    template<typename T> TLSPointer<T>::~TLSPointer() {
        pthread_key_delete(key);
    }

    template<typename T> T* TLSPointer<T>::get() {
        T* result = static_cast<T*> (pthread_getspecific(key));

        if (result == NULL && this->constructor != NULL) {
            result = this->constructor();
            pthread_setspecific(key, static_cast<void *> (result));
        }

        return result;
    }

    template<typename T> void TLSPointer<T>::reset(T* new_value) {
        T* current = static_cast<T*> (pthread_getspecific(key));

        if (new_value != current) {
            // Destroy old value
            if (current != NULL && this->destructor != NULL)
                this->destructor(current);
            pthread_setspecific(key, static_cast<void *> (new_value));
        }
    }

    template<typename T> T* TLSPointer<T>::release() {
        T* temp = static_cast<T*> (pthread_getspecific(key));
        pthread_setspecific(key, NULL);
        return temp;
    }
}

#endif /* EPYX_TLS_POINTER_DETAIL_H */
