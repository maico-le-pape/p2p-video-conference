#include "vpxdecoder.h"
#include "../../src/core/common.h"
#include <vpx/vp8dx.h>
#include <cstring>

namespace Epyx
{
    namespace webm
    {

        VpxDecoder::VpxDecoder() {
            vpx_codec_dec_cfg_t cfg;
            memset(&cfg, 0, sizeof cfg);
            int dec_flags = 0;
            vpx_codec_dec_init(&decoder, &vpx_codec_vp8_dx_algo, &cfg, dec_flags);
        }

        VpxDecoder::~VpxDecoder() {
            vpx_codec_destroy(&decoder);
        }

        bool VpxDecoder::decode(const FramePacket& pkt) {
            if (vpx_codec_decode(&decoder, pkt.data.data(), pkt.data.size(), 0, 0)) {
                log::error << "[VPX decoder] Failed to decode frame: "
                    << vpx_codec_error(&decoder) << log::endl;
                return false;
            }
            return true;
        }
        
        vpx_image_t* VpxDecoder::getFrame() {
            vpx_codec_iter_t iter = NULL;
            return vpx_codec_get_frame(&decoder, &iter);
        }
    }
}
