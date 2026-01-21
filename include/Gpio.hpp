#ifndef GPIO_HPP
#define GPIO_HPP

#include <iostream>

/**
 * @class GpioInterface
 * @details Interface to succesfully compile for both architectures.
 */
class GpioInterface {
public:
  virtual ~GpioInterface() {}
  virtual int init() = 0;
  virtual int setup() = 0;
};

#ifdef RPI
#include <array>
#include <filesystem>
#include <gpiod.hpp>
#include <memory>
#include <optional>
#include <stddef.h>

constexpr size_t NUM_PINS = 40;

/**
 * @class GpioPi
 * @details Real working version for pi itself
 */
class GpioPi : public GpioInterface {
private:
  std::filesystem::path PATH{"/dev/gpiochip4"};
  gpiod::line_settings settings;
  std::optional<gpiod::line_request> request;
  bool keyState = false;
  bool buttonState = false;

public:
  GpioPi();
  int init() override;
  int setup() override;
  void poll();
  void shutdown(gpiod::edge_event event);
  void toggleKey(gpiod::edge_event event);
  bool getKeyState();
  bool getButtonState();
};

// Logical number of pin https://pinout.xyz/pinout/pin29_gpio5/
enum class LogicalPin : unsigned int {
  KEY = 17,
  SHUTDOWN = 27,
};

#else

/**
 * @class GpioMock
 * @details Desktop version of Gpio (STUB)'
 */
class GpioMock : public GpioInterface {
public:
  GpioMock();
  int init() override;
  int setup() override;
  void toggleKey(bool toggle);
};
// End RPI definition
#endif

// Ending header guard
#endif