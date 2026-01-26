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
#include <string_view>
#include <vector>

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
 * @brief Class for handling the Raspberry Pi serial port reading.
 * @details Poll a serial port for incoming weight and present the result to
 * SDL Window.
 */
class Device {
public:
  /**
   * @brief Constructor that initiates state variable and connects to port
   */
  Device();

  /**
   * @brief Destructor that sets the state and joins threads.
   */
  ~Device();

  /**
   * @brief Thread function that runs readFromSerial().
   */
  void pollWeight();

  /**
   * @brief Thread function that runs readTime().
   */
  void pollTime();

  /**
   * @brief Getter for the weight.
   *
   * Gets the current weight in main logic.
   */
  uint16_t getWeight();

  /**
   * @brief Getter for the clock string
   *
   * Gets the current timePoint string
   */
  std::string_view getTimepoint() const;

private:
  /**
   * @brief Convert incoming weight from serial > int.
   */
  uint16_t convertWeight();

  /**
   * @brief Reads fd and until condtion is met.
   *
   * Pushes a weight that is later converted to an int.
   */
  void readFromSerial();

  /**
   * @brief Set the current time point.
   */
  void setTime();

  /**
   * @brief set the current timepoint
   */
  void initiateTime();

  /**
   * @brief Opens fd and configures the serial port.
   *
   * Called in the constructor to start a succesful connection before reading
   * from it.
   *
   * @return true if succesful.
   */
  bool connectToPort();

  /**
   * @brief Configuarion of a port to represent a common RS232
   *
   * @param settings of configured port.
   * @param baud rating of port, must match both ends.
   */
  void configureSerial(termios &settings, int baud);

  /**
   * @brief File descriptor of open port being used.
   */
  int fd;

  /**
   * @brief Threads running
   */
  std::vector<std::thread> workers;

  /**
   * @brief Variable to store the incoming weight
   */
  std::string incomingWeight{};

  /**
   * @brief Variable for thread safe assigning
   */
  std::mutex mutex{};

  /**
   * @brief Variable for storing the converted weight.
   */
  std::atomic<uint16_t> weight{};

  /**
   * @brief State variable used for thread.
   */
  std::atomic<bool> state{};

  /**
   * @brief Thread safe variable for updating the timepoint variable.
   *
   * Will always be presented on the SDL Window.
   */
  std::string timepoint;

  //! Design of the timepoint.
  char timeString[std::size("dd/mm-yy hh:mm")]; // Store converted local time.
};

#endif