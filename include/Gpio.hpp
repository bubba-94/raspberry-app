#ifndef GPIO_HPP
#define GPIO_HPP

#include <iostream>
#include <memory>

/**
 * @class GpioInterface
 * @details Interface to succesfully compile for both architectures.
 */
class GpioInterface {
public:
  virtual ~GpioInterface() {}
  virtual void init() = 0;
  virtual void setup() = 0;
  virtual void toggleKey(bool toggle) = 0;
};

#ifdef RPI
#include <array>
#include <filesystem>
#include <gpiod.hpp>
#include <stddef.h>

constexpr size_t NUM_PINS = 40;
constexpr size_t KEY_SWITCH = 17;

/**
 * @class GpioPi
 * @details Real working version for pi itself
 */
class GpioPi : public GpioInterface {
private:
  std::filesystem::path PATH{"/dev/gpiochip4"};
  gpiod::line_settings settings{};
  gpiod::line_request request{};
  gpiod::request_builder builder{};

public:
  GpioPi();
  void init() override;
  void setup() override;
  void toggleKey(bool toggle) override;
  void configure();
  void printLineInfo(gpiod::line_info line);
  void build(gpiod::line::offset *pin);
};

enum class Pin : size_t { KEY_SWITCH = 17 };

#else

/**
 * @class GpioMock
 * @details Desktop version of Gpio (STUB)'
 */
class GpioMock : public GpioInterface {
public:
  GpioMock();
  void init() override;
  void setup() override;
  void toggleKey(bool toggle) override;
};
// End RPI definition
#endif

// Ending header guard
#endif