#include "Graphics.hpp"

int main() {
  SDLManager sdl;

  int initialWeight = 0;

  sdl.init();
  sdl.setup();

  // While state is true.
  while (sdl.getStatus()) {
    // Queue for events
    sdl.poll();

    initialWeight++;

    sdl.update(initialWeight);
  }

  sdl.shutdown();
  return 0;
}