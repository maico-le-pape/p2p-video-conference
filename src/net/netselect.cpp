#include "netselect.h"
#include <sys/select.h>

namespace Epyx
{

    NetSelect::NetSelect(int numworkers, const std::string& workerName)
    :lastId(1), workers(this), running(true) {
        workers.setName(workerName);
        workers.setNumWorkers(numworkers);
    }

    NetSelect::~NetSelect() {
        // Stop working pool
        workers.stop();

        // Stop running thread
        running = false;

        // Delete every selected stuff
        std::lock_guard<std::mutex> lock(readersMutex);
        readers.clear();
    }

    int NetSelect::add(const std::shared_ptr<NetSelectReader>& nsr) {
        EPYX_ASSERT(nsr != NULL);
        nsr->setOwner(this);
        int id = std::atomic_fetch_add(&lastId, 1);
        std::lock_guard<std::mutex> lock(readersMutex);
        readers[id].reset(new NetSelectReaderInfo(nsr));
        return id;
    }

    void NetSelect::kill(int id) {
        // Get an iterator on a map of unique_ptr to NetSelectReaderInfo
        std::lock_guard<std::mutex> lock(readersMutex);
        auto nsri = readers.find(id);
        if (nsri != readers.end()) {
            nsri->second->alive = false;
            nsri->second->reader->close();
            readers.erase(nsri);
        }
    }

    std::shared_ptr<NetSelectReader> NetSelect::get(int id) {
        // Get an iterator on a map of unique_ptr to NetSelectReaderInfo
        std::lock_guard<std::mutex> lock(readersMutex);
        auto nsri = readers.find(id);
        if (nsri != readers.end() && nsri->second->alive) {
            return nsri->second->reader;
        }
        return std::shared_ptr<NetSelectReader>();
    }

    int NetSelect::getNumWorkers() const {
        return workers.getNumWorkers();
    }

    void NetSelect::setNumWorkers(int n) {
        workers.setNumWorkers(n);
    }

    void NetSelect::run() {
        fd_set rfds;
        struct timeval tv;
        int fdmax;
        while (running) {
            // Build select parameters
            FD_ZERO(&rfds);
            fdmax = 0;
            {
                std::lock_guard<std::mutex> lock(readersMutex);
                for (auto i = readers.begin(); running && i != readers.end(); i++) {
                    NetSelectReaderInfo *nsri = i->second.get();
                    EPYX_ASSERT(nsri != NULL);
                    if (!nsri->alive || nsri->inQueue)
                        continue;
                    EPYX_ASSERT(nsri->reader);
                    int fd = nsri->reader->getFileDescriptor();
                    if (fd >= 0) {
                        FD_SET(fd, &rfds);
                        if (fd > fdmax)
                            fdmax = fd;
                    }
                }
            }
            if (!running)
                return;

            // Refresh every 0.01sec the FD list
            tv.tv_sec = 0;
            tv.tv_usec = 10000;
            int status = select(fdmax + 1, &rfds, NULL, NULL, &tv);
            if (status < -1)
                throw ErrException("NetSelect", "select");

            // Add each FD to the blocking queue
            std::list<int> closedList;
            {
                std::lock_guard<std::mutex> lock(readersMutex);
                for (auto i = readers.begin(); running && i != readers.end(); i++) {
                    NetSelectReaderInfo *nsri = i->second.get();
                    EPYX_ASSERT(nsri != NULL);
                    if (!nsri->alive)
                        continue;
                    EPYX_ASSERT(nsri->reader);
                    int fd = nsri->reader->getFileDescriptor();
                    if (fd >= 0) {
                        if (!nsri->inQueue && FD_ISSET(fd, &rfds)) {
                            nsri->inQueue = true;
                            workers.post(new int(i->first));
                        }
                    } else {
                        closedList.push_back(i->first);
                    }
                }
            }

            // Remove closed sockets
            while (!closedList.empty()) {
                this->kill(closedList.front());
                closedList.pop_front();
            }
        }
    }

    NetSelect::NetSelectReaderInfo::NetSelectReaderInfo(const std::shared_ptr<NetSelectReader>& nsr)
    :reader(nsr), alive(true), inQueue(false) {
    }

    NetSelect::Workers::Workers(NetSelect *owner)
    :owner(owner) {
    }

    void NetSelect::Workers::treat(std::unique_ptr<int> nsriId) {
        EPYX_ASSERT(owner != NULL);
        EPYX_ASSERT(nsriId);

        // Find the reader from the ID
        std::shared_ptr<NetSelectReader> nsr = owner->get(*nsriId);
        if (!nsr)
            return;

        // Unlock readers map when reading data
        if (!nsr->read()) {
            // Destroy this reader on false returned value
            owner->kill(*nsriId);
            return;
        }

        // Remove the queue marcker of this reader
        {
            std::lock_guard<std::mutex> lock(owner->readersMutex);
            auto nsri = owner->readers.find(*nsriId);
            if (nsri != owner->readers.end()) {
                nsri->second->inQueue = false;
            }
        }
    }
}
