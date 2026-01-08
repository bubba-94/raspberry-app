#ifndef DEVICE_HPP
#define DEVICE_HPP

// Serial/terminal communication
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

// C++ Standard
#include <cstdint>
#include <iostream>
#include <string>

// Timestamp
#include <ctime>

// Thread safe specifics
#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

constexpr uint8_t DELAY = 16;
constexpr uint8_t BUFFER_LENGTH = 32;

// Test port
constexpr const char *PORT_A = "/dev/ttyACM0";


/**
 * @class Device
 * @brief Class for handling the Raspberry Pi weight generating
 * @details Poll a serial port for incoming weight (preferably on a separate
 * thread) and present the result to SDL Window.
 */
class Device {
private:
  int port;
  std::time_t timestamp;
  std::thread worker;
  std::string incomingWeight{};
  std::mutex mutex{};
  std::atomic<uint16_t> weight{};
  std::atomic<bool> state{};

public:
  Device();
  ~Device();

  void init();
  void setup();
  void poll();

  bool getState();
  uint16_t getWeight();

  void readFromSerial();
  bool connectToPort();
  void configureSerial(termios &settings, int baud);
  void operator()() const;
  void sleepFor(uint8_t delay);
  void printErrMsg(const char *errMsg);
  uint16_t convertWeight();
};

enum class DeviceErr : std::uint8_t {
  NONE = 0,
  PORT_ERR = 1,
  SERIAL_ERR = 2,
  THREAD_ERR = 3,
  BUFFER_ERR = 4
};

#endif