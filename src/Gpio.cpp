#include "Gpio.hpp"

#ifdef RPI

GpioPi::GpioPi() {}

int GpioPi::init() {
  gpiod::chip chip(PATH);

  if (!chip) {
    std::cerr << "Error opening " << PATH << "\n";
    return -1;
  }

  settings.set_direction(gpiod::line::direction::INPUT);
  settings.set_edge_detection(gpiod::line::edge::BOTH);
  settings.set_active_low(false);

  auto builder = chip.prepare_request();
  builder.add_line_settings(
      gpiod::line::offset{static_cast<unsigned int>(Pin::KEY_SWITCH)},
      settings);

  request = builder.do_request();

  std::cout << "Offset " << static_cast<unsigned int>(Pin::KEY_SWITCH)
            << " initialized\n";
  return 0;
}

int GpioPi::setup() { return 0; }

void GpioPi::poll() {
  if (!request)
    return;

  if (!request->wait_edge_events(std::chrono::milliseconds{0}))
    return;

  gpiod::edge_event_buffer buffer;

  request->read_edge_events(buffer);

  for (const auto &event : buffer) {
    if (event.line_offset() == static_cast<unsigned int>(Pin::KEY_SWITCH)) {

      if (event.type() == gpiod::edge_event::event_type::RISING_EDGE) {
        keyState = true;
      } else {
        keyState = false;
      }
    }
  }
}

bool GpioPi::getState() { return keyState; }

void GpioPi::toggleKey(bool toggle) {
  toggle = !toggle;
  if (toggle) {
    std::cout << "Line " << toggle;
  } else {
    std::cout << "Line " << toggle;
  }
}

#else

GpioMock::GpioMock() {}

int GpioMock::init() {
  std::cout << "[STUB] Mock chip initialized" << "\n";
  return 0;
}

int GpioMock::setup() {
  std::cout << "[STUB] Mock setup initialized" << "\n";
  return 0;
}

void GpioMock::toggleKey(bool toggle) {
  toggle = !toggle;
  std::cout << "[STUB] Mock toggle is: " << toggle;
}

#endif