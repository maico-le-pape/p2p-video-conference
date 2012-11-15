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
 * @file exception.h
 * @brief Epyx exceptions
 */
#ifndef EPYX_EXCEPTION_H
#define EPYX_EXCEPTION_H

#include <string>

namespace Epyx
{
    /**
     * @class Exception
     * @brief Epyx raw exception
     */
    class Exception : public std::exception
    {
    public:
        /**
         * @brief Build an exception
         * @param type
         * @param module
         * @param message
         */
        Exception(const char *type, const char *module, const char *message);

        /**
         * @brief Append text to the message
         * @param txt text to append
         */
        void append(const std::string& txt);

        /**
         * @brief Print an exception
         * @param os output stream
         * @param e
         */
        friend std::ostream& operator<<(std::ostream& os, const Exception& e);

        /**
         * @brief Get message
         */
        const std::string& getMessage() const;

        /**
         * @brief std::exception inheritance
         * @return message
         */
        virtual const char* what() const throw();

    protected:
        /**
         * @brief Exception type
         */
        std::string type;

        /**
         * @brief Module which throws the exception
         */
        std::string module;

        /**
         * @brief Exception text
         */
        std::string message;
    };
    /**
     * @class FailException
     * @brief Epyx failure
     */
    class FailException : public Exception
    {
    public:
        /**
         * @brief Build an "Epyx fails" exception
         * @param module
         * @param message
         */
        FailException(const char *module, const char *message);
    };
    /**
     * @class ErrException
     * @brief View standard errors (errno and errstr) in an exception
     */
    class ErrException : public FailException
    {
    public:
        /**
         * @brief Build an standard error exception
         * @param module
         * @param fct failed function name
         */
        ErrException(const char *module, const char *fct);
    };
    /**
     * @class MinorException
     * @brief Internal Epyx exception (meant to be caught at some point)
     */
    class MinorException : public Exception
    {
    public:
        /**
         * @brief Build an internal Epyx exception
         * @param module
         * @param message error message to be logged
         */
        MinorException(const char *module, const char *message);
    };
    /**
     * @class ParserException
     * @brief Exception which is raised when invalid data is parsed
     */
    class ParserException : public Exception
    {
    public:
        /**
         * @brief Build a parser exception
         * @param module
         * @param message
         */
        ParserException(const char *module, const char *message);
    };
}

#endif /* EPYX_EXCEPTION_H */
