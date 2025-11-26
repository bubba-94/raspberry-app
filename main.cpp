/**
 * Basic SDL2 application that i instruct to run on boot when starting my
 * Raspberry Pi 5. Going to be used later on for a sort of scaling device for
 * vehicles.
 */

// 
#include "SDL.hpp"

int main() {

  Window wind;
  Window *wind_ptr = &wind;
  EventManager manager;
  Renderer rend(*wind_ptr);

  wind.createWindow("Pi window", 1080, 640);

  while (wind.getState()) {
    // Queue for events
    manager.poll();

    while (manager.hasEvent()) {

      // Create wrapper in event manager?
      SDL_Event e = manager.getNext();

      switch (e.type) {

      
      case SDL_QUIT:
        std::cout << "Closing SDL Window " << '\n';
        wind.shutdown();
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

  wind.shutdown();

  return 0;
}