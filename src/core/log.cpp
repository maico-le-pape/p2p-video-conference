#include "log.h"
#include <errno.h>
#include <cstring>
#include <cstdlib>

namespace Epyx {
namespace log {

    TLSPointer<BufferContainer>* _buffers = nullptr;
    EndlStruct endl;
    ErrstdStruct errstd;
    bool initialized = false;
    Worker _worker;

    static BufferContainer* create_buffers(){
        return new BufferContainer;
    }

    void init(int flags, const std::string& file){
        EPYX_ASSERT_NO_LOG(Thread::isInitialized());
        _worker.init(flags, file);
        _buffers = new TLSPointer<BufferContainer>(create_buffers);
        initialized = true;
    }

    void flush(){
        _worker.flush(false);
    }

    void waitFlush(){
        _worker.flush(true);
    }
    void flushAndQuit(){
        _worker.quit();
    }

    Stream::Stream(int prio): priority(prio) {}

    Stream::~Stream(){}

    Stream& Stream::operator<<(const EndlStruct& f) {
        EPYX_ASSERT_NO_LOG(log::initialized);

        //Give the log to the worker and flush the buffer
        std::ostringstream* buffer = &_buffers->get()->b[this->priority];
        _worker.write(buffer->str(), this->priority);
        buffer->str("");

        return *this;
    }

    Stream& Stream::operator<<(const ErrstdStruct& f) {
        EPYX_ASSERT_NO_LOG(log::initialized);

        //At least on Linux, errno is thread-safe
        char buffer[64];
        strerror_r(errno, buffer, sizeof(buffer));
        *this << buffer << " (errno #" << errno << ")";

        return *this;
    }
}
}
