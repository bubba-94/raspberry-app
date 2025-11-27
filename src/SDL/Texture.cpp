#include "SDL/Texture.hpp"
#include "SDL/Render.hpp"
#include "SDL/Surface.hpp"

void Texture::create(Renderer &renderer, Surface &surface){
    texture.reset(
        SDL_CreateTextureFromSurface(
        renderer.getRenderer(), 
        surface.getSurface()));
}