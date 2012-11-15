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
 * @file assert.h
 * @brief Assert() macro implementation with Epyx logger
 *
 * You need to include log.h and exception.h to use this file
 */
#ifndef EPYX_ASSERT_H
#define EPYX_ASSERT_H

#include <iostream>

/**
 * @def EPYX_VERIFY(condition)
 * An assert using Epyx' logging system, always active.
 */
#define EPYX_VERIFY(condition)\
{\
    if(!(bool)(condition)){\
        Epyx::log::fatal << "\n[ASSERT] " #condition "\n";\
        Epyx::log::fatal << "    - Function: " << __FUNCTION__ << " \n";\
        Epyx::log::fatal << "    - File: " << __FILE__ << " : " << __LINE__ << Epyx::log::endl;\
        throw Epyx::FailException("assert", #condition);\
    }\
}

/**
 * @def EPYX_ASSERT(condition)
 * An assert which may be disabled in production.
 */
#define EPYX_ASSERT(condition) EPYX_VERIFY(condition)

/**
 * @def EPYX_ASSERT_WARN(condition)
 * An assert using Epyx' logging system. It does not throw an exception thus it only warns about the error.
 */
#define EPYX_ASSERT_WARN(condition)\
{\
    if(!(bool)(condition)){\
        Epyx::log::fatal << "\n[ASSERT] " #condition "\n";\
        Epyx::log::fatal << "    - Function: " << __FUNCTION__ << " \n";\
        Epyx::log::fatal << "    - File: " << __FILE__ << " : " << __LINE__ << Epyx::log::endl;\
    }\
}

/**
 * @def EPYX_ASSERT_NO_LOG(condition)
 * An assert that does not use Epyx' logging system (for use in the logging system).
 */
#define EPYX_ASSERT_NO_LOG(condition)\
{\
    if(!(bool)(condition)){\
        std::cerr << "[ASSERT] " #condition "\n";\
        std::cerr << "    - Function: " << __FUNCTION__ << " \n";\
        std::cerr << "    - File: " << __FILE__ << " : " << __LINE__ << std::endl;\
        throw Epyx::FailException("assert", #condition);\
    }\
}

#endif /* EPYX_ASSERT_H */
