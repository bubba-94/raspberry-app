#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

/// C++ Standard Library
#include <cstring>
#include <gpiod.h>
#include <iostream>
#include <memory>
#include <queue>
#include <string>

// File to keep this file
#include "SDLManagerTypes.hpp"

/// Windows specs for Raspberry Pi Monitor
constexpr Uint16 WINDOW_WIDTH = 1920;
constexpr Uint16 WINDOW_HEIGHT = 1080;

// Image paths ()
#ifdef RPI
constexpr const char *LOGO =
    "/home/johan/Programs/pay-per-weigh/aarch64/img/pandema.png";
constexpr const char *IMAGE =
    "/home/johan/Programs/pay-per-weigh/aarch64/img/qr.png";
constexpr const char *FONT =
    "/home/johan/Programs/pay-per-weigh/aarch64/fonts/Lato-Light.ttf";
constexpr const char *WINDOW_TITLE = "Pay Per Weigh";

#else
constexpr const char *LOGO =
    "/home/moodin/coding/internship/raspberry-app/img/pandema.png";
constexpr const char *IMAGE =
    "/home/moodin/coding/internship/raspberry-app/img/qr.png";
constexpr const char *FONT =
    "/home/moodin/coding/internship/raspberry-app/fonts/Lato-Light.ttf";
constexpr const char *WINDOW_TITLE = "Desktop Test";

#endif
// Surface size
constexpr Uint16 FONT_CHAR_SIZE = 250;
constexpr Uint16 FONT_HEIGHT = 500;
constexpr Uint16 IMAGE_WIDTH = 500;
constexpr Uint16 IMAGE_HEIGHT = 500;
constexpr Uint16 LOGO_WIDTH = 242;
constexpr Uint16 LOGO_HEIGHT = 48;

// Font position (centered)
constexpr Uint16 FONT_Y = ((WINDOW_HEIGHT / 2) + FONT_HEIGHT / 2) - FONT_HEIGHT;

// Image position (centered)
constexpr Uint16 IMAGE_X = ((WINDOW_WIDTH / 2) + IMAGE_WIDTH / 2) - IMAGE_WIDTH;
constexpr Uint16 IMAGE_Y =
    (WINDOW_HEIGHT / 2) + (IMAGE_HEIGHT / 2) - IMAGE_HEIGHT;

// Logo position (bottom right) with some spacing
constexpr Uint16 LOGO_X = WINDOW_WIDTH - LOGO_WIDTH - 50;
constexpr Uint16 LOGO_Y = WINDOW_HEIGHT - LOGO_HEIGHT - 50;
constexpr int MAX_WEIGHT = 15001;

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

public:
  // Main logic
  SDLError init();
  void poll(bool state);
  void setup();
  void render(int weight);
  void shutdown();

  /*-------------PRINTS-------------*/
  void printErrMsg(const char *errMsg);

  // Image functions
  int loadImage();

  // Checks
  bool checkWeight(int weight);
  int checkLengthInChar(int weight);
  bool getStatus();

  // Setups
  void setSurfacePosition(SDLSpec *surface, Uint16 x, Uint16 y, Uint16 w,
                          Uint16 h);
  void setFontWidth(int weight);
  void setRenderingColor(Uint8 r, Uint8 g, Uint8 b);
  void drawImageFrame();

  // Creation of needed SDL instances
  SDLError createWindow();
  SDLError createRenderer();

  // Used for updating the weight
  SDLError updateFontSurface(int newWeight);

  // Image and Logo are SDL_Surfaces, called once in setup
  SDLError loadSurfaceOfIMG(const char *filepath);
  SDLError loadFontSurface(const char *filepath, int startWeight);
  SDLError createImageTexture(SDL_Surface *surface);
  SDLError createLogoTexture(SDL_Surface *surface);
  SDLError createFontTexture(SDL_Surface *surface);

  // Get raw instance pointers
  SDL_Window *getRawWindow() const;
  SDL_Renderer *getRawRenderer() const;
  SDL_Surface *getRawSurface() const;
  SDL_Texture *getRawImage() const;
  SDL_Texture *getRawLogo() const;
  SDL_Texture *getRawWeight() const;
  TTF_Font *getRawFont() const;

  // Events, will not be used later
  bool hasEvent() const;
  void pollEvents();
  SDL_Event getNext();

private:
  // Settings of surfaces
  SDLSpec logoSpec;
  SDLSpec qrSpec;
  SDLSpec weightSpec;

  // Smart pointers for SDL instances
  sdl_unique<SDL_Window> window;
  sdl_unique<SDL_Surface> surface;
  sdl_unique<SDL_Renderer> renderer;
  sdl_unique<SDL_Texture> logo;   // Texture for logo (always visible)
  sdl_unique<SDL_Texture> image;  // Texture for QR code
  sdl_unique<SDL_Texture> weight; // Texture for weight
  sdl_unique<TTF_Font> font;

  int fontWidth = 0; // Adjustable during runtime
  int fontX = 0;     // X cursor of font

  bool status = true;    // State of the SDL Window
  bool showImage = true; // State to decide what image is shown

  SDL_Event event; // Check for single event
  std::queue<SDL_Event> events;
};

#endif