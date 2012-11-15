#include "timeout.h"
#include "assert.h"
#include "log.h"

namespace Epyx
{

    Timeout::Timeout(unsigned int ms) {
        struct timeval tv;
        int gettimeofday_status = gettimeofday(&tv, NULL);
        EPYX_ASSERT(gettimeofday_status == 0);

        long usec = tv.tv_usec + (1000 * ms);
        maxTime.tv_usec = usec % 1000000;
        maxTime.tv_sec = tv.tv_sec + (usec / 1000000);
    }

    bool Timeout::hasExpired() const {
        struct timeval tv;
        int gettimeofday_status = gettimeofday(&tv, NULL);
        EPYX_ASSERT(gettimeofday_status == 0);

        return (tv.tv_sec > maxTime.tv_sec) ||
            (tv.tv_sec == maxTime.tv_sec && tv.tv_usec > maxTime.tv_usec);
    }

    struct timeval Timeout::remainingTimeval() const {
        struct timeval tv;
        int gettimeofday_status = gettimeofday(&tv, NULL);
        EPYX_ASSERT(gettimeofday_status == 0);

        if (tv.tv_sec > maxTime.tv_sec) {
            // Timeout in seconds
            tv.tv_sec = 0;
            tv.tv_usec = 0;
            return tv;
        }
        tv.tv_sec = maxTime.tv_sec - tv.tv_sec;
        if (tv.tv_usec > maxTime.tv_usec) {
            if (tv.tv_sec == 0) {
                // Timeout in microseconds
                tv.tv_usec = 0;
                return tv;
            }
            // Time difference with a reminder
            tv.tv_sec--;
            tv.tv_usec = 1000000 + maxTime.tv_usec - tv.tv_usec;
        } else {
            tv.tv_usec = maxTime.tv_usec - tv.tv_usec;
        }
        return tv;
    }

    int Timeout::remainingMsec() const {
        struct timeval tv = this->remainingTimeval();
        return tv.tv_sec * 1000 + tv.tv_usec % 1000;
    }

    bool operator<(const Timeout& t1, const Timeout& t2){
        if (t1.maxTime.tv_sec < t2.maxTime.tv_sec) return true;
        if (t1.maxTime.tv_sec > t2.maxTime.tv_sec) return false;
        return t1.maxTime.tv_usec < t2.maxTime.tv_usec;
    }
}
