#ifdef RPI
#ifndef PINSTATE_HPP
#define PINSTATE_HPP

/**
 * @class PinState
 *
 * @brief States used by Graphics and Gpio
 *
 * @details
 * A boolean that controls certain states read by Gpio and
 * later forwarded to SDLManager.
 */
struct PinState {
  bool shutdownRequested = false;
  bool keyEnabled = false;
};

#endif
#endif