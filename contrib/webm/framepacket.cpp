#include "framepacket.h"
#include "../../src/core/common.h"
#include <cstring>

namespace Epyx
{
    namespace webm
    {

        FramePacket::FramePacket(byte_str& data, unsigned long timestamp,
            unsigned long duration)
        :timestamp(timestamp), duration(duration) {
            this->data.swap(data);
        }

        FramePacket::FramePacket(GTTPacket& pkt) {
            //Check the packet is not ill-formed
            if (pkt.protocol.compare("WEBM") || pkt.method.compare("FRAME_PACKET")) {
                throw ParserException("WebM::FramePacket", "Invalid packet");

            }
            if (pkt.headers.count("duration") == 0 || pkt.headers.count("timestamp") == 0 || pkt.body.empty()) {
                throw ParserException("WebM::FramePacket", "Invalid packet headers");
            }

            //Copy everything from the GTT packet
            timestamp = String::toULong(pkt.headers["timestamp"]);
            duration = String::toULong(pkt.headers["duration"]);
            // Swap packet body
            data.swap(pkt.body);
        }

        byte_str FramePacket::build() const {
            GTTPacket pkt;
            pkt.protocol = "WEBM";
            pkt.method = "FRAME_PACKET";
            pkt.headers["duration"] = String::fromUnsignedLong(duration);
            pkt.headers["timestamp"] = String::fromUnsignedLong(timestamp);
            pkt.body = data;
            return pkt.build();
        }
    }
}
