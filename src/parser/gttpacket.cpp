#include "gttpacket.h"
#include "../core/common.h"
#include <sstream>
#include <cstring>

namespace Epyx
{

    std::ostream& operator<<(std::ostream& os, const GTTPacket& pkt) {
        os << "[Proto " << pkt.protocol << " method " << pkt.method << std::endl;
        for (std::map<std::string, std::string>::const_iterator i = pkt.headers.begin();
            i != pkt.headers.end(); ++i) {
            os << "  " << (*i).first << ": " << (*i).second << std::endl;
        }
        if (pkt.body.size() > 0) {
            os << "  content-length: " << pkt.body.size() << std::endl;
        }
        os << "]";
        return os;
    }

    byte_str GTTPacket::build() const {
        std::stringstream head;

        // First line
        head << protocol << " " << method << String::crlf;

        // Headers
        for (auto it = headers.begin(); it != headers.end(); it++) {
            head << it->first << ": " << it->second << String::crlf;
        }
        if (body.size() > 0) {
            head << "content-length: " << body.size() << String::crlf;
        }
        head << String::crlf;

        // Concatenate headers and content
        std::string header = head.str();
        return string2bytes(header).append(body);
    }
}
