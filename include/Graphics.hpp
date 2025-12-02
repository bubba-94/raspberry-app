#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

/// SDL libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/// C++ Standard Library
#include <iostream>
#include <memory>
#include <queue>
#include <string>

/// Defines to keep WindowApp clean
#include "SDLTypes.hpp"

/// Windows specs
constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;
constexpr int WIDTH_PADDING = WINDOW_WIDTH - 200;
constexpr int HEIGHT_PADDING = WINDOW_HEIGHT - 200;
constexpr const char *WINDOW_TITLE = "SDLManager Window";

/**
 *
 * @class SDLManager
 * @brief SDL Library wrapper for image rendering.
 * @returns A manager that owns ptrs to SDL_Window, SDL_Render, SDL_Surface,
 * SDL_Texture.
 * @details Class that wraps simple SDL functions to create QR code rendering.
 *
 */
class SDLManager {
private:
  sdl_unique<SDL_Window> window;
  sdl_unique<SDL_Renderer> renderer;
  sdl_unique<SDL_Surface> surface;
  sdl_unique<SDL_Texture> texture;

  /// @brief  Used for debuggning, probably not needed later
  std::queue<SDL_Event> events;

  /// @brief  Bool to track the state of manager
  bool state = true;

public:
  // Init specific
  SDLError createWindow();
  SDLError createRenderer();
  SDLError createSurface();
  SDLError createTexture();

  // Calls other inits
  void init();
  void shutdown();

  // Prints
  void printErrMsg(const char *errMsg);

  // Getters
  SDL_Window *getRawWindow() const;
  SDL_Renderer *getRawRenderer() const;
  SDL_Surface *getRawSurface() const;
  SDL_Texture *getRawTexture() const;

  // Events, will not be used later
  bool getState();
  void poll();
  bool hasEvent() const;
  SDL_Event getNext();
};

#endif