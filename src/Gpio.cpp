#include "Gpio.hpp"

#ifdef RPI

void GpioPi::init() {
  gpio::chip chip(PATH)

      if (!chip) {
    std::cerr << "Error initializing " << PATH << "\n";
    return 1;
  }
}

void

#else

#endif