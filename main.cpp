#include "Graphics.hpp"

int main() {
  SDLManager sdl;
  bool image = false;
  int initialWeight = 0;
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
        std::cout << "Switching texture: \n";
        image = !image;
        break;
      default:
        break;
      }
      initialWeight++;
    }

    sdl.presentWindow(image, initialWeight);
    SDL_Delay(100);
  }

  sdl.shutdown();
  return 0;
}