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
 * @file byte.h
 * @brief Define a convenient type to deal with bunary data
 */
#ifndef EPYX_CORE_BYTE_H
#define EPYX_CORE_BYTE_H

#include <string>

namespace Epyx
{
    /**
     * @brief A byte must be an unsigned 8-bit integer
     */
    typedef unsigned char byte;
    
    /**
     * @brief String of bytes are like vectors, with size field
     */
    typedef std::basic_string<byte> byte_str;
}

// Conversion macros
#define string2bytes(str) (reinterpret_cast< ::Epyx::byte_str&>(str))
#define bytes2string(bytes) (reinterpret_cast<std::string&>(bytes))

// Const macros
#define string2bytes_c(str) (reinterpret_cast<const ::Epyx::byte_str&>(str))
#define bytes2string_c(bytes) (reinterpret_cast<const std::string&>(bytes))

#endif /* EPYX_CORE_BYTE_H */
