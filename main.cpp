/**
 * Basic SDL2 application that i instruct to run on boot when starting my
 * Raspberry Pi 5. Going to be used later on for a sort of scaling device for
 * vehicles.
 */

// Has to be linked.
#include "EventManager.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include <iostream>

int main() {

  Window wind;
  Window *ptr = &wind;
  EventManager manager;
  Renderer rend(*ptr);

  wind.createWindow("Pi window", 1920, 1080);

  while (wind.getState()) {
    // Queue for events
    manager.poll();

    while (manager.hasEvent()) {

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