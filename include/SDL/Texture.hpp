#ifndef TEXTURE_HPP
#define TEXTURE_HPP

// Include SDL_Texture class
#include <SDL2/SDL_render.h>

#include <memory>

class Surface;
class Renderer;

class Texture {
    private:
        struct DelTexture {
        void operator()(SDL_Texture *ptr) const {
        if (ptr)
                SDL_DestroyTexture(ptr);
            }
        };
        
        std::unique_ptr<SDL_Texture, DelTexture> texture;

    public:
        Texture() = default;
        ~Texture() = default;
        void create(Renderer &renderer, Surface &surface);
};

#endif