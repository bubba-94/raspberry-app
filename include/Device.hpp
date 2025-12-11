#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <termios.h> // Serial configuration
#include <unistd.h> // Terminal sys calls
#include <fcntl.h> // Permissions for file handling

// C++ Standard
#include <string>
#include <cstdint>
#include <iostream>

#include <ctime>

// Thread safe specifics
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>

constexpr uint16_t DELAY = 300;

/**
 * @class Device
 * @brief Class for handling the Raspberry Pi weight generating
 * @details Poll a serial port for incoming weight (preferably on a separate thread) and present the result to SDL Window.
 */
class Device
{
    private:
        std::time_t currentTime;  
        std::thread worker;
        std::string incomingWeight {}; 
        std::mutex mutex {}; 
        std::atomic<int> weight {};
        std::atomic<bool> state{};

    public:
        Device();
        ~Device();

        void init();
        void setup();
        void poll();
        void join(std::thread &thread);
        bool getState();

        void operator()() const;
        void sleepFor(uint16_t delay);
        std::string convertWeight(uint16_t weight);

};

#endif