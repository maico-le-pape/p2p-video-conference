/**
 * @file webm/videoframe.h
 * @brief VideoFrame definition
 */
#ifndef EPYX_WEBM_VIDEOFRAME
#define EPYX_WEBM_VIDEOFRAME

#include <string>
#include <vpx/vpx_image.h>
#include <SDL/SDL.h>
#include "../../src/core/common.h"

namespace Epyx
{
namespace webm
{
    /**
     * @class VideoFrame
     * @brief A screen that simply shows a vpx_image
     */
    class VideoFrame
    {
    public:
        /**
         * @brief the VideoFrame constructor
         * @param width the initial width of the window
         * @param height the initial height of the window
         * @param name the window name
         */
        VideoFrame(int width, int height, const std::string& name);
        ~VideoFrame();

        /**
         * @brief actually creates the window
         * @return true if it succeded, false otherwise
         */
        bool init();

        /**
         * @brief shows the given image in the frame
         * @param image the image to be shown
         */
        void showFrame(const vpx_image_t* image);

        /**
         * @brief handle SDL events especially the quit event
         * @return true if the user asked to quit, false otherwise
         */
        bool checkSDLQuitAndEvents();

    private:
        static const int video_format = SDL_ANYFORMAT | SDL_DOUBLEBUF | SDL_RESIZABLE;
        std::mutex sdl_lock;
        int width, height;
        std::string window_name;
        SDL_Rect screen_rect;
        SDL_Surface* screen;
        SDL_Overlay* overlay;
    };

}
}

#endif //EPYX_WEBM_VIDEOFRAME
