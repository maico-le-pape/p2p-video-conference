#include "vpxencoder.h"
#include "../../src/core/common.h"
#include <vpx/vp8cx.h>

namespace Epyx
{
    namespace webm
    {

        VpxEncoder::VpxEncoder(unsigned int display_width, unsigned int display_height,
            unsigned int video_bitrate)
        :display_width(display_width), display_height(display_height),
        video_bitrate(video_bitrate) {
            int cpu_used = -6;
            int static_threshold = 1200;
            vpx_codec_enc_cfg_t cfg;

            vpx_codec_enc_config_default(&vpx_codec_vp8_cx_algo, &cfg, 0);
            cfg.rc_target_bitrate = video_bitrate;
            cfg.g_w = display_width;
            cfg.g_h = display_height;
            cfg.g_timebase.num = 1;
            cfg.g_timebase.den = (int) 10000000;
            cfg.rc_end_usage = VPX_CBR;
            cfg.g_pass = VPX_RC_ONE_PASS;
            cfg.g_lag_in_frames = 0;
            cfg.rc_min_quantizer = 20;
            cfg.rc_max_quantizer = 50;
            cfg.rc_dropframe_thresh = 1;
            cfg.rc_buf_optimal_sz = 1000;
            cfg.rc_buf_initial_sz = 1000;
            cfg.rc_buf_sz = 1000;
            cfg.g_error_resilient = 1;
            cfg.kf_mode = VPX_KF_DISABLED;
            cfg.kf_max_dist = 999999;
            cfg.g_threads = 1;
            vpx_codec_enc_init(&encoder, &vpx_codec_vp8_cx_algo, &cfg, 0);
            vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, cpu_used);
            vpx_codec_control_(&encoder, VP8E_SET_STATIC_THRESHOLD, static_threshold);
            vpx_codec_control_(&encoder, VP8E_SET_ENABLEAUTOALTREF, 0);
        }

        VpxEncoder::~VpxEncoder() {
            vpx_codec_destroy(&encoder);
        }

        bool VpxEncoder::encode(const vpx_image_t& raw, unsigned long time_ms,
            unsigned int flags) {
            vpx_codec_encode(&encoder, &raw, time_ms * 1000, 30000000, flags, VPX_DL_REALTIME);
            if (encoder.err) {
                log::error << "[VPX encoder] Failed to encode frame: "
                    << vpx_codec_error(&encoder) << log::endl;
                return false;
            }
            // Reset iteration
            iter = NULL;
            return true;
        }

        std::unique_ptr<FramePacket> VpxEncoder::getPacket() {
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter))) {
                // Only use frame packets
                if (pkt->kind != VPX_CODEC_CX_FRAME_PKT)
                    continue;
                byte_str data((const byte*) pkt->data.frame.buf, pkt->data.frame.sz);
                return std::unique_ptr<FramePacket>(new FramePacket(data,
                    pkt->data.frame.pts, pkt->data.frame.duration));
            }
            // End of iteration
            iter = NULL;
            return std::unique_ptr<FramePacket>();
        }
    }
}
