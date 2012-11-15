/**
 * @file webm/videoframe.cpp
 * @brief VideoFrame definition
 */
#include "videoframe.h"
#include "../../src/core/common.h"

namespace Epyx
{
namespace webm
{

    VideoFrame::VideoFrame(int width, int height, const std::string& name)
    :width(width), height(height), window_name(name) {
        screen_rect.x = 0;
        screen_rect.y = 0;
        screen_rect.w = width;
        screen_rect.h = height;
    }

    VideoFrame::~VideoFrame() {
        SDL_FreeYUVOverlay(overlay);
        SDL_Quit();
    }

    bool VideoFrame::init() {
        log::info << "Starting the SDL subsystem..." << log::endl;

        int result = SDL_Init(SDL_INIT_VIDEO);
        if (result < 0) {
            log::error << "Could not start SDL (error: " << result << ")" << log::endl;
            return false;
        }

        char driverName[128];
        SDL_VideoDriverName(driverName, sizeof (driverName));
        log::info << "Using Video Driver : " << driverName << log::endl;

        screen = SDL_SetVideoMode(width, height, 0, video_format);
        overlay = SDL_CreateYUVOverlay(width, height, SDL_YV12_OVERLAY, screen);

        SDL_WM_SetCaption(window_name.c_str(), NULL);

        return true;
    }

    void VideoFrame::showFrame(const vpx_image_t* image) {
        std::lock_guard<std::mutex> lock(sdl_lock);
        SDL_LockYUVOverlay(overlay);

        unsigned char *frame_pointer = (unsigned char *) overlay->pixels[0];

        unsigned char *inputY = image->planes[VPX_PLANE_Y];
        int iY = 0;
        while (iY++ < height) {
            memcpy(frame_pointer, inputY, width);

            inputY += image->stride[VPX_PLANE_Y];
            frame_pointer += width;
        }

        unsigned char *inputU = image->planes[VPX_PLANE_U];
        int iU = 0;
        while (iU++ < height / 2) {
            memcpy(frame_pointer, inputU, width / 2);

            inputU += image->stride[VPX_PLANE_U];
            frame_pointer += width / 2;
        }

        unsigned char *inputV = image->planes[VPX_PLANE_V];
        int iV = 0;
        while (iV++ < height / 2) {
            memcpy(frame_pointer, inputV, width / 2);

            inputV += image->stride[VPX_PLANE_V];
            frame_pointer += width / 2;
        }

        SDL_UnlockYUVOverlay(overlay);
        SDL_DisplayYUVOverlay(overlay, &screen_rect);
    }

    bool VideoFrame::checkSDLQuitAndEvents() {
        std::lock_guard<std::mutex> lock(sdl_lock);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    log::info << "SDL_QUIT event received" << log::endl;
                    return true;

                case SDL_VIDEORESIZE:
                    screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 0, video_format);
                    screen_rect.w = event.resize.w;
                    screen_rect.h = event.resize.h;
            }
        }

        return false;
    }

}
}

