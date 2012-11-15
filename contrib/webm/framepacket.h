/**
 * @file webm/framepacket.h
 */

#ifndef EPYX_WEBM_FRAMEPACKET_H
#define EPYX_WEBM_FRAMEPACKET_H

#include <boost/noncopyable.hpp>
#include <stddef.h>
#include "../../src/core/byte.h"
#include "../../src/parser/gttpacket.h"

namespace Epyx
{
    namespace webm
    {
        /**
         * @brief Packet for a video frame
         */
        class FramePacket : private boost::noncopyable
        {
        public:
            /**
             * @brief Full constructor
             * @param data Binary data which is SWAPPED with internal buffer
             * @param timestamp
             * @param duration
             */
            FramePacket(byte_str& data, unsigned long timestamp, unsigned long duration);

            /**
             * @brief Constructor from a GTT packet
             * @param pkt
             * 
             * NOTE: At the end of this function, pkt.body is empty
             */
            FramePacket(GTTPacket& pkt);

            /**
             * @brief Build raw bytes to be sended
             * @return 
             */
            byte_str build() const;

            /**
             * @brief Packet data
             */
            byte_str data;

            /**
             * Timestamp in miliseconds
             */
            unsigned long timestamp;

            /**
             * @brief Packet duration
             */
            unsigned long duration;
        };
    }
}

#endif /* EPYX_WEBM_FRAMEPACKET_H */

