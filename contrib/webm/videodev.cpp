#include "videodev.h"
#include "../../src/core/common.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cstring>

namespace Epyx
{
    namespace webm
    {

        /**
         * UYVY to YV12 conversion
         * @param yv12 destination image
         * @param uyvy source image
         * @param w
         * @param h
         */
        static void uyvy2yv12(byte *yv12, const byte *uyvy,
            unsigned int w, unsigned int h) {
            byte *y = yv12;
            byte *u = y + (w * h);
            byte *v = w / 2 * h / 2 + u;
            unsigned int i, j;

            const byte *p = uyvy;
            for (; y < u; p += 4) {
                *y++ = p[0];
                *y++ = p[2];
            }

            p = uyvy;
            for (i = 0; i < (h >> 1); i++, p += (w << 1))
                for (j = 0; j < (w >> 1); j++, p += 4)
                    * u++ = p[1];

            p = uyvy;
            for (i = 0; i < (h >> 1); i++, p += (w << 1))
                for (j = 0; j < (w >> 1); j++, p += 4)
                    * v++ = p[3];
        }

        VideoDev::VideoDev(unsigned int display_width, unsigned int display_height,
            unsigned int capture_frame_rate, unsigned int video_bitrate)
        :display_width(display_width), display_height(display_height),
        capture_frame_rate(capture_frame_rate), video_bitrate(video_bitrate),
        fd(-1) {
            for (unsigned int i = 0; i < NB_BUFFER; i++) {
                mem[i] = NULL;
                mem_size[i] = 0;
            }
            count_captured_frames = 0;
        }

        VideoDev::~VideoDev() {
            this->close();
        }

        void VideoDev::close() {
            if (fd < 0)
                return;
            this->stop_capture();
            ::close(fd);
            fd = -1;
        }

        bool VideoDev::start_capture() {
            if (fd >= 0) {
                this->stop_capture();
            }
            fd = open("/dev/video0", O_RDWR | O_NONBLOCK);
            if (fd < 0) {
                log::warn << "[VideoDev] Unable to open /dev/video0: " << log::errstd << log::endl;
                return false;
            }

            //Ask for the capabilities of the device
            struct v4l2_capability cap;
            memset(&cap, 0, sizeof (struct v4l2_capability));
            if (ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0) {
                log::warn << "[VideoDev] Unable to query capabilities: " << log::errstd << log::endl;
                return false;
            }

            //Set the format (size and pixel format)
            struct v4l2_format fmt;
            memset(&fmt, 0, sizeof (struct v4l2_format));
            fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            fmt.fmt.pix.width = display_width;
            fmt.fmt.pix.height = display_height;
            fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
            fmt.fmt.pix.field = V4L2_FIELD_ANY;
            if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0) {
                log::warn << "[VideoDev] Unable to set format: " << log::errstd << log::endl;
                return false;
            }

            //Set the streaming parameters
            struct v4l2_streamparm setfps;
            memset(&setfps, 0, sizeof (struct v4l2_streamparm));
            setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            setfps.parm.capture.timeperframe.numerator = 1;
            setfps.parm.capture.timeperframe.denominator = capture_frame_rate;
            if (ioctl(fd, VIDIOC_S_PARM, &setfps) < 0) {
                log::warn << "[VideoDev] Unable to set stream param: " << log::errstd << log::endl;
                return false;
            }

            //Request MemoryMapping with NB_BUFFER buffers
            struct v4l2_requestbuffers rb;
            memset(&rb, 0, sizeof (struct v4l2_requestbuffers));
            rb.count = NB_BUFFER;
            rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            rb.memory = V4L2_MEMORY_MMAP;
            if (ioctl(fd, VIDIOC_REQBUFS, &rb) < 0) {
                log::warn << "[VideoDev] Unable to set request buffers: " << log::errstd << log::endl;
                return false;
            }

            // Map the buffers
            for (unsigned int i = 0; i < NB_BUFFER; i++) {
                struct v4l2_buffer buf;
                memset(&buf, 0, sizeof (struct v4l2_buffer));
                buf.index = i;
                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_MMAP;
                if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) {
                    log::warn << "[VideoDev] Unable to query buffer " << i
                        << ": " << log::errstd << log::endl;
                    return false;
                }

                mem[i] = mmap(0, buf.length, PROT_READ, MAP_SHARED, fd, buf.m.offset);
                if (mem[i] == MAP_FAILED) {
                    log::warn << "[VideoDev] Error mapping buffer " << i
                        << ": " << log::errstd << log::endl;
                    return false;
                }
                mem_size[i] = buf.length;
            }

            // Queue the buffers
            for (unsigned int i = 0; i < NB_BUFFER; ++i) {
                struct v4l2_buffer buf;
                memset(&buf, 0, sizeof (struct v4l2_buffer));
                buf.index = i;
                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_MMAP;
                if (ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
                    log::warn << "[VideoDev] Unable to queue buffer " << i
                        << ": " << log::errstd << log::endl;
                    return false;
                }
            }

            // start streaming
            int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            if (ioctl(fd, VIDIOC_STREAMON, &type) < 0) {
                log::warn << "[VideoDev] Unable to start streaming: " << log::errstd << log::endl;
                return false;
            }
            return true;
        }

        bool VideoDev::stop_capture() {
            // Don't stop if there's no file
            if (fd < 0)
                return false;

            int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            if (ioctl(fd, VIDIOC_STREAMOFF, &type) < 0) {
                // No need to log, because this may happen when there is a problem at start
                //log::warn << "[VideoDev] Unable to stop streaming: " << log::errstd << log::endl;
                return false;
            }

            // Unmap buffers
            for (unsigned int i = 0; i < NB_BUFFER; i++) {
                if (mem_size[i] > 0 && mem[i] != NULL && mem[i] != MAP_FAILED) {
                    munmap(mem[i], mem_size[i]);
                    mem_size[i] = 0;
                    mem[i] = NULL;
                }
            }
            return true;
        }

        bool VideoDev::get_frame(vpx_image_t *raw) {
            EPYX_ASSERT(raw != NULL);
            struct v4l2_buffer buf;
            memset(&buf, 0, sizeof (struct v4l2_buffer));
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;

            // get a frame if we can
            if (ioctl(fd, VIDIOC_DQBUF, &buf) < 0) {
                if (errno == EAGAIN) {
                    // Busy
                    return false;
                }
                log::warn << "[VideoDev] Unable to dequeue buffer: " << log::errstd << log::endl;
                return false;
            }

            // super ugly conversion :)
            if (buf.bytesused > 0) {
                uyvy2yv12(raw->img_data, (const byte*) mem[buf.index],
                    display_width, display_height);
            }

            // put the buffer back
            if (ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
                log::warn << "[VideoDev] Unable to requeue buffer: " << log::errstd << log::endl;
                return false;
            }

            count_captured_frames++;
            return true;
        }
    }
}
