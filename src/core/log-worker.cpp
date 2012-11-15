#include "log.h"
#include <string>

namespace Epyx
{
namespace log
{

    Worker::~Worker() {
        delete thread;
    }

    void Worker::init(int flags, const std::string& file) {
        this->flags = flags;

        if (!file.empty() && (this->flags & LOGFILE)) {
            //TODO: Close it
            this->logFile.open(file.c_str());
            EPYX_ASSERT_NO_LOG(logFile.is_open())
        }

        // start thread
        thread = new std::thread(&Worker::run, this);
    }

    void Worker::write(const std::string& message, int prio) {
        this->entries.push(new LogEntry{message, prio, time(NULL), Thread::getName(), nullptr});
    }

    void Worker::flush(bool wait) {
        LogEntry* entry = new LogEntry{"", FLUSH, 0, "", nullptr};

        //When we are told to wait we will create and wait on a
        //condition that will be triggered by the logger
        //TODO: what if the logger exits while still holding a condition?
        if (wait) {
            entry->cond = new std::condition_variable();
            auto cond = entry->cond;

            {
                std::mutex m;
                std::unique_lock<std::mutex> lock(m);
                this->entries.push(entry);
                cond->wait(lock);
            }

            //The entry will get destroyed but not the condition
            delete cond;
        } else {
            this->entries.push(entry);
        }
    }

    void Worker::quit() {
        this->entries.push(new LogEntry{"", QUIT, 0, "", nullptr});
        this->thread->join();
    }

    std::string logLevelNames[5] = {
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL"
    };

    void Worker::run() {
        while (1) {
            //Take all the elements from the queue with only one call
            auto entries = this->entries.flush();
            if (entries == nullptr) continue;

            while (!entries->empty()) {
                std::unique_ptr<LogEntry> entry;
                entry.swap(entries->front());

                //Handles control messages
                if (entry->prio == FLUSH || entry->prio == QUIT) {

                    //Flush only the streams we are using
                    if (this->flags & CONSOLE) std::cout << std::flush;
                    if (this->flags & ERRORCONSOLE) std::cerr << std::flush;
                    if (this->flags & LOGFILE) logFile << std::flush;

                    //Unlock the thread that asked the flush
                    if (entry->cond != nullptr) {
                        entry->cond->notify_one();
                    }

                    if (entry->prio == QUIT) {
                        //TODO clean up
                        return;
                    }
                } else {
                    this->printEntry(*entry);
                }

                entries->pop_front();
            }
        }
    }

    void Worker::printEntry(const LogEntry& entry) {
        //As this function is called from only one thread I can use static variables to avoid allocations
        static char time_buffer[20];
        static char date_buffer[20];

        //This returns a pointer to a static struct, I hope I'm the only one using it
        //TODO: fix this!
        tm* timeinfo = localtime(&entry.time);
        strftime(time_buffer, 20, "%H:%M:%S", timeinfo);
        strftime(date_buffer, 20, "%Y-%m-%d", timeinfo);

        //Make the part with the thread's name
        std::ostringstream info_buffer;
        info_buffer << "[" << logLevelNames[entry.prio] << "] ";
        info_buffer << "[" << entry.thread_name << "]";

        //TODO: Flush control commands
        //Do the actual IO
        if (this->flags & CONSOLE) {
            std::cout << "[" << time_buffer << "] " << info_buffer.str() << " " << entry.str << "\n";
        }
        if (this->flags & ERRORCONSOLE) {
            std::cerr << "[" << time_buffer << "] " << info_buffer.str() << " " << entry.str << "\n";
        }
        if (this->flags & LOGFILE) {
            logFile << "[" << date_buffer << " " << time_buffer << "] " << info_buffer.str() << " " << entry.str << "\n";
        }
    }

}
}
