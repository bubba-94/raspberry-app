#include "SDL/Render.hpp"
#include "SDL/Window.hpp"

Renderer::Renderer(Window &window) {
  renderer.reset(
      SDL_CreateRenderer(window.getWindow(), -1, SDL_RENDERER_ACCELERATED));
}

SDL_Renderer* Renderer::getRenderer(){
  return renderer.get();
}