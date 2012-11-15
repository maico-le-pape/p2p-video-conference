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
 * @file name-struct.h
 * @brief Definition of the name struct which can be used to create types from string literals
 */

#ifndef EPYX_CORE_NAME_STRUCT_H
#define EPYX_CORE_NAME_STRUCT_H

#include "name-struct-detail.h"

namespace Epyx
{
    struct NameStructBase{};

    template<uint32_t I> struct NameStruct: public NameStructBase {
        static const uint32_t hash = I;
        static const NameStruct<I> instance;
    };

    #define EPYX_NAME_STRUCT(str) Epyx::NameStruct<compileTimeHash(str)>
    #define EPYX_NAME_STRUCT_INSTANCE(str) EPYX_NAME_STRUCT(str)::instance
}
#endif //EPYX_CORE_NAME_STRUCT_H

