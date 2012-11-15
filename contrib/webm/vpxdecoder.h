/**
 * @file webm/vpxdecoder.h
 * @brief Abstraction layer to vpx codec decoder
 */

#ifndef EPYX_WEBM_VPX_DECODER_H
#define EPYX_WEBM_VPX_DECODER_H

#include <vpx/vpx_decoder.h>
#include "framepacket.h"

namespace Epyx
{
    namespace webm
    {
        /**
         * @brief Decode a FramePacket to several vpx_image_t
         */
        class VpxDecoder
        {
        public:
            VpxDecoder();
            ~VpxDecoder();

            /**
             * @brief Decode a packet
             * @param pkt frame packet
             */
            bool decode(const FramePacket& pkt);

            /**
             * @brief Get a frame, or return NULL
             * @return
             * 
             * NOTE: The returned image should not be free by the caller
             */
            vpx_image_t* getFrame();

        private:
            vpx_codec_ctx_t decoder;
        };
    }
}

#endif /* EPYX_WEBM_VPX_DECODER_H */
