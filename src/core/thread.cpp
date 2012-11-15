#include "thread.h"
#include "assert.h"
#include "log.h"
#include "exception.h"

namespace Epyx
{
    TLSPointer<std::string>* detail::thread_name = NULL;
    bool detail::thread_initialized = false;

    Thread::Thread()
    :thread(0) {
    }

    Thread::Thread(const std::string& name, int id)
    :thread(0) {
        this->setThreadName(name, id);
    }

    Thread::~Thread() {
        if (this->thread != 0) {
            this->term();
        }
    }

    void Thread::setThreadName(const std::string& name, int id) {
        if (id != -1) {
            std::ostringstream str;
            str << name << " " << id;
            this->name = str.str();
        } else {
            this->name = name;
        }
    }

    const std::string& Thread::getThisName() {
        return name;
    }

    bool Thread::start() {
        int thread_create_status = pthread_create(&(this->thread), NULL,
            Thread::_thread_start, this);
        EPYX_ASSERT(thread_create_status == 0);
        EPYX_ASSERT(this->thread != 0);
        return true;
    }

    void Thread::wait() {
        EPYX_ASSERT(this->thread != 0);
        int thread_join_status = pthread_join(this->thread, NULL);
        EPYX_ASSERT(thread_join_status == 0);
    }

    void Thread::term() {
        if (this->thread != 0) {
            int thread_cancel_status = pthread_cancel(this->thread);
            // Thread may have disappeared
            EPYX_ASSERT(thread_cancel_status == 0 || thread_cancel_status == ESRCH);
            this->thread = 0;
        }
    }

    void Thread::setName(const std::string& name) {
        detail::thread_name->reset(new std::string(name));
    }

    std::string Thread::getName() {
        std::string* name = detail::thread_name->get();
        if (name == nullptr) {
            return "Unnamed";
        } else {
            return *name;
        }
    }

    void Thread::init(const std::string& name, int id) {
        detail::thread_name = new TLSPointer<std::string>();

        Thread::setName(name);

        detail::thread_initialized = true;
    }

    bool Thread::isInitialized() {
        return detail::thread_initialized;
    }

    void* Thread::_thread_start(void *arg) {
        try {
            // Call Thread::run()
            Epyx::Thread *self = (Epyx::Thread*) arg;
            EPYX_ASSERT(self != NULL);
            Thread::setName(self->name);
            self->run();
        } catch (std::exception& e) {
            log::fatal << "Thread exception !" << log::endl;
            log::fatal << e.what() << log::endl;
        }
        return NULL;
    }
}
