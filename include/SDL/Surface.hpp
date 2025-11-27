#ifndef SURFACE_HPP
#define SUFRACE_HPP

// Include SDL_Surface
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

#include <memory>

class Surface {
    private:
        struct DelSurface {
        void operator()(SDL_Surface *ptr) const {
        if (ptr)
                SDL_FreeSurface(ptr);
            }
        };

        std::unique_ptr<SDL_Surface, DelSurface> surface;
    public:
        Surface() = default;
        ~Surface() = default;
        
        void create(int height, int width);
        void loadPNG(const char *file);

        SDL_Surface* getSurface();

    };

#endif