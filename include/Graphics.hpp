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

/// header to keep WindowApp clean
#include "SDLTypes.hpp"

/// Windows specs
constexpr Uint16 WINDOW_WIDTH = 1920;
constexpr Uint16 WINDOW_HEIGHT = 1080;
constexpr const char *WINDOW_TITLE = "SDLManager Window";

// Image paths ()
constexpr const char *LOGO =
    "/home/moodin/coding/internship/raspberry-app/img/logo.png";
constexpr const char *IMAGE =
    "/home/moodin/coding/internship/raspberry-app/img/qr.png";

// Surface sizes
constexpr Uint16 IMAGE_WIDTH = 400;
constexpr Uint16 IMAGE_HEIGHT = 400;
constexpr Uint16 LOGO_WIDTH = 225;
constexpr Uint16 LOGO_HEIGHT = 225;

// Middle of the screen (centered)
constexpr Uint16 IMAGE_X = ((WINDOW_WIDTH / 2) + IMAGE_WIDTH / 2) - IMAGE_WIDTH;
constexpr Uint16 IMAGE_Y =
    (WINDOW_HEIGHT / 2) + (IMAGE_HEIGHT / 2) - IMAGE_HEIGHT;

// Logo position  (should be bottom right)
constexpr Uint16 LOGO_X = WINDOW_WIDTH - LOGO_WIDTH;
constexpr Uint16 LOGO_Y = WINDOW_HEIGHT - LOGO_HEIGHT;

// Background
constexpr Uint8 RED = 255;
constexpr Uint8 GREEN = 255;
constexpr Uint8 BLUE = 255;

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
  SDL_Rect frame;
  sdl_unique<SDL_Window> window;
  sdl_unique<SDL_Surface> surface;
  sdl_unique<SDL_Renderer> renderer;
  sdl_unique<SDL_Texture> logo;
  sdl_unique<SDL_Texture> image;

  //  Used for debugging, probably not needed later
  std::queue<SDL_Event> events;

  /// @brief  Bool to track the state of manager
  bool state = true;

public:
  /**
   * @brief Sets up the environment for the window.
   * @return SDLError if failed
   */
  SDLError init();
  void shutdown();

  /*-------------PRINTS-------------*/
  void printErrMsg(const char *errMsg);

  /*-------------SDL SPECIFIC-------------*/

  // Creation of needed SDL instances
  SDLError createWindow();
  SDLError createRenderer();

  // Image and Logo are SDL_Surfaces
  SDLError loadSurfaceOfIMG(const char *filepath);

  // Requires an SDL_Surface (image).
  SDLError createImageTexture(SDL_Surface *surface);
  SDLError createLogoTexture(SDL_Surface *surface);

  // Get raw instance pointers
  SDL_Window *getRawWindow() const;
  SDL_Renderer *getRawRenderer() const;
  SDL_Surface *getRawSurface() const;
  SDL_Texture *getRawImage() const;
  SDL_Texture *getRawLogo() const;

  // Image functions
  int loadLogo();
  int loadImage();
  void presentWindow();

  /**
   * @brief wrapper around sdl clear and draw color.
   * @param r amount of red
   * @param g amount of green
   * @param b amount of blue
   */
  void setBackground(Uint8 r, Uint8 g, Uint8 b);
  void drawImageFrame();

  // Events, will not be used later
  bool getState();
  void poll();
  bool hasEvent() const;
  SDL_Event getNext();
};

#endif