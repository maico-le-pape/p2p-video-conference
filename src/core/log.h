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
 * @file log.h
 * @brief A thread safe logging system.
 */
#ifndef EPYX_LOG_H
#define EPYX_LOG_H

#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include "log-worker.h"
#include "tls-pointer.h"
#include "assert.h"

namespace Epyx {
namespace log {

    //And this is the ofstream of each stream
    struct BufferContainer {
        std::ostringstream b[5];
    };

    extern TLSPointer<BufferContainer>* _buffers;
    extern bool initialized;
    extern Worker _worker;

    //Debug levels
    enum {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,

        FLUSH,
        QUIT
    };

    //Defines a Stream class to handle << operators nicely
    //This Struct is the endLog struct that ends the log line
    // (a log line can be multiple lines)
    struct EndlStruct{};
    struct ErrstdStruct{};

    /**
     * @brief An agument to be passed to a log stream to finish a log entry
     *
     * A log entry can span across multiple lines
     */
    extern EndlStruct endl;

    /**
     * @brief An agument to be passed to a log stream to print the standard error code nicely
     */
    extern ErrstdStruct errstd;

    /**
     * @class Stream
     * @brief The definition of a logging stream to have a nice interface
     */
    class Stream {
    private:
        std::ostringstream buffer;
        int priority;

        Stream(const Stream&);
        const Stream& operator=(const Stream&);

    public:
        /**
         * @brief Constructor
         * @param prio stream priority
         */
        Stream(int prio);

        /**
         * @brief Destructor
         */
        ~Stream();

        /**
         * @brief overloads << to accept almost anything
         */
        template<typename T> Stream& operator<<(const T& arg);
        /**
         * @brief Message terminator
         * @param f EndlStruct
         * @return this
         */
        Stream& operator<<(const EndlStruct& f);
        /**
         * @brief Add standard error to log message
         * @param f ErrstdStruct
         * @return this
         */
        Stream& operator<<(const ErrstdStruct& f);
    };

    //Here the definition of the different log streams
    /**
     * @brief The debug log stream
     */
    static Stream debug(DEBUG);

    /**
     * @brief The info log stream
     */
    static Stream info(INFO);

    /**
     * @brief The warn log stream
     */
    static Stream warn(WARN);

    /**
     * @brief The error log stream
     */
    static Stream error(ERROR);

    /**
     * @brief The fatal log stream
     */
    static Stream fatal(FATAL);

    //End of the definition of Streams

    //Allow to put pretty much everything in a log
    template<typename T> Stream& Stream::operator<<(const T& arg) {
        EPYX_ASSERT_NO_LOG(log::initialized);
        _buffers->get()->b[this->priority] << arg;
        return *this;
    }


    //Entry point for the logger
    enum {
        CONSOLE = 1,
        ERRORCONSOLE = 2,
        LOGFILE = 4
    };

    /**
     * @brief initializes the logger
     * @param flags the initialisation flags
     * @param file (optional) the file to log to
     */
    void init(int flags, const std::string& file = "");

    /**
     * @brief Ask the logging system to flush the logs to the logfile and the log streams -- Non-blocking variant
     *
     * The flags are CONSOLE, ERRORCONSOLE and LOGFILE
     */
    void flush();

    /**
     * @brief Ask the logging system to flush the logs to the logfile and the log streams -- Blocking variant
     */
    void waitFlush();

    /**
     * @brief Quits the logger nicely
     */
    void flushAndQuit();

}
}

#endif /* EPYX_LOG_H */
