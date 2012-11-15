/**
 * @file webm/videodev.h
 * @brief Acquire video from V4L device, /dev/video0
 */

#ifndef EPYX_WEBM_VIDEODEV_H
#define EPYX_WEBM_VIDEODEV_H

#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <linux/videodev2.h>

namespace Epyx
{
    namespace webm
    {
        /**
         * @brief Video device interaction
         */
        class VideoDev
        {
        public:
            /**
             * @brief Number of buffer for capture
             */
            static const unsigned int NB_BUFFER = 4;

            /**
             * @brief Initialize everything
             * @param display_width
             * @param display_height
             * @param capture_frame_rate
             * @param video_bitrate
             */
            VideoDev(unsigned int display_width, unsigned int display_height,
                    unsigned int capture_frame_rate = 30, unsigned int video_bitrate = 400);

            ~VideoDev();

            /**
             * @brief Close file descriptor
             */
            void close();

            /**
             * @brief Start a live capture
             * @return true on success
             */
            bool start_capture();

            /**
             * @brief Stop a live capture
             * @return true on success
             */
            bool stop_capture();

            /**
             * @brief Get a frame
             * @param raw pointer to a vpx_image_t structure
             * @return true on success
             */
            bool get_frame(vpx_image_t *raw);

        private:
            unsigned int display_width;
            unsigned int display_height;
            unsigned int capture_frame_rate;
            unsigned int video_bitrate;

            // File descriptor
            int fd;

            void* mem[NB_BUFFER];
            size_t mem_size[NB_BUFFER];

            unsigned int count_captured_frames;
        };
    }
}

#endif /* EPYX_WEBM_VIDEODEV_H */

