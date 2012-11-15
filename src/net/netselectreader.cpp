#include "netselectreader.h"
#include "../core/common.h"

namespace Epyx
{

    NetSelectReader::NetSelectReader()
    :owner(NULL) {
    }

    NetSelectReader::~NetSelectReader() {
    }

    void NetSelectReader::close() {
        int fd = this->getFileDescriptor();
        if (fd >= 0) {
            ::close(fd);
        }
    }

    NetSelect* NetSelectReader::getOwner() const {
        EPYX_ASSERT(owner != NULL);
        return owner;
    }

    void NetSelectReader::setOwner(NetSelect *ns) {
        EPYX_ASSERT(owner == NULL);
        owner = ns;
    }
}
