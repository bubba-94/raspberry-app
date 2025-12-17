#include "Device.hpp"
#include "Graphics.hpp"

int main() {
  SDLManager sdl;
  Device pi;

  int currentWeight{0};
  sdl.init();
  sdl.setup();
  pi.init();

  // While state is true.
  while (sdl.getStatus()) {

    sdl.poll();
    sdl.update(currentWeight);
    currentWeight = pi.getWeight();
  }

  sdl.shutdown();
  return 0;
}