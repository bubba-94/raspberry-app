#include "Device.hpp"
#include "Gpio.hpp"
#include "Graphics.hpp"

int main() {
  SDLManager sdl;
  Device pi;

#ifdef RPI
  GpioPi gpio;
#else
  GpioMock gpio;
#endif

  int currentWeight{0};
  sdl.init();
  sdl.setup();
  pi.init();
  gpio.init();

  // While state is true.
  while (sdl.getStatus()) {

#ifdef RPI
    currentWeight = pi.getWeight();
    gpio.poll();
    sdl.poll(gpio.getState());
#else
    sdl.pollEvents();
    currentWeight = 1337;
#endif

    sdl.render(currentWeight);
  }

  sdl.shutdown();
  return 0;
}