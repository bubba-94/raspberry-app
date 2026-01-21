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
      gpiod::line::offset{static_cast<unsigned int>(LogicalPin::KEY)},
      settings);
  builder.add_line_settings(
      gpiod::line::offset{static_cast<unsigned int>(LogicalPin::SHUTDOWN)},
      settings);

  request = builder.do_request();

  std::cout << "Offset " << static_cast<unsigned int>(LogicalPin::KEY)
            << " initialized\n";
  std::cout << "Offset" << static_cast<unsigned int>(LogicalPin::SHUTDOWN)
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
    //   if (event.line_offset() == static_cast<unsigned int>(Pin::KEY_SWITCH))
    //   {

    //     if (event.type() == gpiod::edge_event::event_type::RISING_EDGE) {
    //       keyState = true;
    //     } else {
    //       keyState = false;
    //     }
    //   }

    auto pin = static_cast<unsigned int>(event.line_offset());

    switch (pin) {
    case static_cast<unsigned int>(LogicalPin::SHUTDOWN):
      shutdown(event);
      break;
    case static_cast<unsigned int>(LogicalPin::KEY):
      toggleKey(event);
      break;
    }
  }
}

void GpioPi::shutdown(gpiod::edge_event event) {
  auto rising = gpiod::edge_event::event_type::RISING_EDGE;
  if (event.type() == rising) {
    buttonState = true;
  } else {
    buttonState = false;
  }
}

void GpioPi::toggleKey(gpiod::edge_event event) {
  auto rising = gpiod::edge_event::event_type::RISING_EDGE;

  if (event.type() == rising) {
    keyState = true;
  } else {
    keyState = false;
  }
}

bool GpioPi::getKeyState() { return keyState; }
bool GpioPi::getButtonState() { return buttonState; }

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