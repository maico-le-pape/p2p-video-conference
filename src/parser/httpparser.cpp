#include "httpparser.h"
#include "../core/common.h"
#include <string.h>

namespace Epyx
{

    void HTTPParser::parseFirstLine(const std::string& line) {
        const char *l = line.c_str();
        EPYX_ASSERT(line.length() != 0);

        // Read HTTP version
        int i = 0;
        while (l[i] != ' ') {
            if (!(isalnum(l[i]) || (l[i] == '/') || (l[i] == '.')))
                throw ParserException("HTTPParser", "Invalid HTTP version");
            i++;
        }
        currentPkt->protocol = std::string(l, i);
        i++;

        // Read HTTP code
        int iCode = i;
        while (l[i] != ' ') {
            if (!(isdigit(l[i])))
                throw ParserException("HTTPParser", "Invalid HTTP code");
            i++;
        }
        currentPkt->method = std::string(l + iCode, i - iCode);
        i++;

        // Read HTTP code description
        int iDesc = i;
        while (l[i] != 0) {
            if (!(isalnum(l[i]) || (l[i] == ' ')))
                throw ParserException("HTTPParser", "Invalid HTTP code decription");
            i++;
        }
        currentPkt->headers["HTTP-code"] = std::string(l + iDesc, i - iDesc);
    }

    void HTTPParser::parseHeaderLine(const std::string& line) {
        const char *l = line.c_str();
        int i = 0, iValue = 0;
        EPYX_ASSERT(line.length() != 0);

        // Read flag name
        if (!isalpha(l[i]))
            throw ParserException("HTTPParser", "flag_name should begin with [a-zA-Z] or we should add newline to end the header");
        i++;
        while (l[i] != ':') {
            if (!(isalnum(l[i]) || (l[i] == '_') || (l[i] == '-')))
                throw ParserException("HTTPParser", "flag name should continue with [A-Za-z0-9-_]* or end with a space");
            i++;
        }
        std::string flagName(l, i);
        String::toLower(flagName);
        i++;

        // Skip spaces
        while (l[i] == ' ')
            i++;
        if (l[i] == 0) {
            //throw ParserException("HTTPParser", "flag without value"); // Why should it bug?
            currentPkt->headers[flagName] = "";
            return;
        }
        // Read value
        iValue = i;
        while (l[i] != 0) {
            if (!(l[i] >= 32 && l[i] < 126))
                throw ParserException("HTTPParser", "flag_value should consist of printable characters or just end with CRLF");
            i++;
        }
        std::string flagValue(l + iValue, i - iValue);

        // Content length
        if (!strcasecmp(flagName.c_str(), "content-length")) {
            if (currentSize > 0)
                throw ParserException("HTTPParser", "content-length flag has already appeared");
            currentSize = String::toInt(flagValue);
            if (currentSize <= 0)
                throw ParserException("HTTPParser", "not valid body size, body size should be a positive integer");
        } else {
            currentPkt->headers[flagName] = flagValue;
        }
    }
}
