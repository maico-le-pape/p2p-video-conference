#include "uri.h"
#include "../core/string.h"
#include <sstream>

namespace Epyx
{
    URI::URI()
    :scheme(""), addr(), path("") {
    }

    URI::URI(const std::string& scheme, const SockAddress& addr, const std::string& path)
    :scheme(scheme), addr(addr), path(path) {
        String::toLower(this->scheme);
    }

    URI::URI(const std::string& uri)
    :scheme(""), addr(), path("") {
        std::string location;

        // Extract scheme
        size_t pos = uri.find("://");
        if (pos != std::string::npos) {
            scheme = uri.substr(0, pos);
            location = uri.substr(pos + 3);
        } else {
            location = uri;
        }
        String::toLower(scheme);

        // Extract address
        pos = location.find('/');
        if (pos != std::string::npos) {
            addr = SockAddress(location.substr(0, pos));
            path = location.substr(pos);
        } else {
            addr = SockAddress(location);
        }
    }

    /**
     * @brief Copy constructor
     * @param uri
     */
    URI::URI(const URI& uri)
    :scheme(uri.scheme), addr(uri.addr), path(uri.path) {
        String::toLower(this->scheme);
    }

    /**
     * @brief Assignment constructor
     * @param uri
     */
    URI& URI::operator=(const URI& uri) {
        if (this != &uri) {
            this->scheme = uri.scheme;
            this->addr = uri.addr;
            this->path = uri.path;
        }
        String::toLower(this->scheme);
        return *this;
    }

    /**
     * @brief Get schema
     * @return this->schema
     */
    const std::string& URI::getScheme() const {
        return scheme;
    }
    /**
     * @brief Get address
     * @return this->addr
     */
    const SockAddress& URI::getAddress() const {
        return addr;
    }
    /**
     * @brief Get absolute path
     * @return this->path
     */
    const std::string& URI::getPath() const {
        return path;
    }

    /**
     * @brief Print the address in an output stream
     * @param os output stream
     * @param uri
     */
    std::ostream& operator<<(std::ostream& os, const URI& uri) {
        if (!uri.scheme.empty())
            os << uri.scheme << "://";
        os << uri.addr << uri.path;
        return os;
    }

    /**
     * @brief get a std::string representation of the URI
     */
    std::string URI::toString() const {
        std::ostringstream stream;
        stream << *this;
        return stream.str();
    }
}
