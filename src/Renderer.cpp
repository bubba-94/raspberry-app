#include "Renderer.hpp"
#include "Window.hpp"

Renderer::Renderer(Window &window) {
  renderer.reset(
      SDL_CreateRenderer(window.getWindow(), -1, SDL_RENDERER_ACCELERATED));
}