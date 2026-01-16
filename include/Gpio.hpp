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
  virtual void toggleKey(bool toggle) = 0;
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

public:
  GpioPi();
  int init() override;
  int setup() override;
  void poll();
  void toggleKey(bool toggle) override;
  bool getState();
};

enum class Pin : unsigned int {
  KEY_SWITCH = 17,
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
  void toggleKey(bool toggle) override;
};
// End RPI definition
#endif

// Ending header guard
#endif