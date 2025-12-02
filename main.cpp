#include "include/Graphics.hpp"

int main() {
  SDLManager sdl;

  sdl.init();

  while (sdl.getState()) {
    // Queue for events
    sdl.poll();

    while (sdl.hasEvent()) {

      // Create wrapper in event manager?
      SDL_Event e = sdl.getNext();

      switch (e.type) {

      case SDL_QUIT:
        std::cout << "Closing SDL Window " << '\n';
        sdl.shutdown();
        break;

      case SDL_KEYDOWN:
        std::cout << "Key pressed: " << SDL_GetKeyName(e.key.keysym.sym)
                  << '\n';
        break;

      case SDL_MOUSEBUTTONDOWN:
        std::cout << "Mouse clicked at: " << e.button.x << ", " << e.button.y
                  << '\n';
        break;
      default:
        break;
      }
    }
    SDL_Delay(16);
  }

  sdl.shutdown();

  return 0;
}