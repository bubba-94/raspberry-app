#include "SDL/Surface.hpp"

void Surface::create(int heigth, int width){
    surface.reset(SDL_CreateRGBSurface(
        0,
        // Width and height of the surface
        width,
        heigth,
        // Depth of bits
        32,
        // Color masks
        0,0,0,0
    ));
}

SDL_Surface* Surface::getSurface(){ return surface.get(); }

void Surface::loadPNG(const char *file){
    surface.reset(IMG_Load(file));
}