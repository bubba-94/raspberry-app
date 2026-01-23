#include "Gpio.hpp"

#ifdef RPI

GpioPi::GpioPi() {}

int GpioPi::init() {
  gpiod::chip chip(PATH);

  if (!chip) {
    std::cerr << "Error opening " << PATH << "\n";
    return -1;
  }

  gpiod::request_builder builder = chip.prepare_request();

  setup(builder);

  request = builder.do_request();

  std::cout << "Offset " << static_cast<unsigned int>(LogicalPin::KEY)
            << " initialized\n";
  std::cout << "Offset " << static_cast<unsigned int>(LogicalPin::SHUTDOWN)
            << " initialized\n";
  return 0;
}

void GpioPi::setup(gpiod::request_builder &builder) {

  settings.set_direction(gpiod::line::direction::INPUT);
  settings.set_edge_detection(gpiod::line::edge::BOTH);
  settings.set_active_low(false);

  builder.add_line_settings(
      gpiod::line::offset{static_cast<unsigned int>(LogicalPin::KEY)},
      settings);
  builder.add_line_settings(
      gpiod::line::offset{static_cast<unsigned int>(LogicalPin::SHUTDOWN)},
      settings);
}

void GpioPi::poll() {

  if (!request || !request->wait_edge_events(std::chrono::milliseconds{0}))
    return;

  gpiod::edge_event_buffer buffer;
  request->read_edge_events(buffer);

  // GPIO Event buffer
  for (const auto &event : buffer) {

    auto pin =
        static_cast<LogicalPin>(static_cast<unsigned int>(event.line_offset()));

    switch (pin) {

    case LogicalPin::SHUTDOWN:
      handleShutdown(event);
      break;
    case LogicalPin::KEY:
      handleKey(event);
      break;
    }
  }
}

void GpioPi::handleShutdown(const gpiod::edge_event &event) {
  state.shutdownRequested =
      event.type() == gpiod::edge_event::event_type::RISING_EDGE;
}

void GpioPi::handleKey(const gpiod::edge_event &event) {
  state.keyEnabled = event.type() == gpiod::edge_event::event_type::RISING_EDGE;
}

const PinState GpioPi::getState() const { return state; }

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