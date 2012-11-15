#include "string.h"
#include "cstring"
#include "assert.h"
#include "log.h"
#include <stdlib.h>
#include <errno.h>
#include <algorithm>

namespace Epyx
{
    const char String::spaces[] = " \t\n\v\f\r";
    const std::string String::crlf("\r\n");
    const byte_str String::crlf_bytes((byte*) "\r\n");

    std::string& String::ltrim(std::string& str) {
        str.erase(0, str.find_first_not_of(spaces));
        return str;
    }

    std::string& String::rtrim(std::string& str) {
        str.erase(str.find_last_not_of(spaces) + 1);
        return str;
    }

    std::string& String::trim(std::string& str) {
        return ltrim(rtrim(str));
    }

    std::string& String::toUpper(std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(),
            std::ptr_fun<int, int>(std::toupper));
        return str;
    }

    std::string& String::toLower(std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(),
            std::ptr_fun<int, int>(std::tolower));
        return str;
    }

    long String::toInt(const std::string& s) {
        std::stringstream ss(s);
        long l;
        try {
            ss >> l;
            if (ss.fail())
                throw new std::exception();
        } catch (std::exception& e) {
            throw ParserException("String::toInt", "Invalid characters");
        }
        return l;
    }

    unsigned long String::toULong(const std::string& s) {
        std::stringstream ss(s);
        unsigned long l;
        try {
            ss >> l;
            if (ss.fail())
                throw new std::exception();
        } catch (std::exception& e) {
            throw ParserException("String::toInt", "Invalid characters");
        }
        return l;
    }

    std::string String::fromInt(int n) {
        std::stringstream out;
        out << n;
        return out.str();
    }

    std::string String::fromUnsignedLong(unsigned long n) {
        std::stringstream out;
        out << n;
        return out.str();
    }
}
