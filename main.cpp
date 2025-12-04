#include "include/Graphics.hpp"

int main() {
  SDLManager sdl;
  bool image = false;
  sdl.init();

  while (sdl.getState()) {
    // Queue for events
    sdl.poll();

    // For debugging and wont be needed later.
    while (sdl.hasEvent()) {

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
        image = !image;
        break;
      default:
        break;
      }
    }

    sdl.presentWindow(image);
    SDL_Delay(50);
  }

  sdl.shutdown();
  return 0;
}