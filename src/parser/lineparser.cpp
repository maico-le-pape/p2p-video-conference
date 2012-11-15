#include "lineparser.h"
#include "../core/common.h"
#include <string.h>

namespace Epyx
{

    LineParser::LineParser()
    :buffer() {
    }

    void LineParser::reset() {
        buffer.clear();
    }

    void LineParser::push(const byte_str& data) {
        if (buffer.empty()) {
            buffer.assign(data);
        } else {
            buffer.append(data);
        }
    }

    bool LineParser::popLine(std::string& line) {
        size_t i = 0;
        for (auto it = buffer.begin(); it != buffer.end(); ++it, ++i) {
            if (*it == '\n') {
                size_t iend = (i > 0 && *(it - 1) == '\r') ? i - 1 : i;
                line.assign((const char*) buffer.substr(0, iend).c_str(), iend);
                buffer.erase(buffer.begin(), it + 1);
                return true;
            }
        }
        return false;
    }

    bool LineParser::popData(byte_str *data, size_t size) {
        EPYX_ASSERT(size > 0 && data != NULL);

        // Not enough bytes in the buffer
        if (size > buffer.size())
            return false;

        // Read data
        data->assign(buffer.substr(0, size));
        buffer.erase(0, size);
        return true;
    }
}
