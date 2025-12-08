#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

/// C++ Standard Library
#include <cstring>
#include <iostream>
#include <memory>
#include <queue>
#include <string>

// File to keep this file
#include "SDLManagerTypes.hpp"

/// Windows specs
constexpr Uint16 WINDOW_WIDTH = 1920;
constexpr Uint16 WINDOW_HEIGHT = 1080;
constexpr const char *WINDOW_TITLE = "SDLManager Window";

// Image paths ()
constexpr const char *LOGO =
    "/home/moodin/coding/internship/raspberry-app/img/pandema.png";
constexpr const char *IMAGE =
    "/home/moodin/coding/internship/raspberry-app/img/qr.png";
constexpr const char *FONT =
    "/home/moodin/coding/internship/raspberry-app/fonts/Lato-Light.ttf";

// Surface size
extern volatile int fontWidth;
constexpr Uint16 FONT_HEIGHT = 500;
constexpr Uint16 IMAGE_WIDTH = 500;
constexpr Uint16 IMAGE_HEIGHT = 500;
constexpr Uint16 LOGO_WIDTH = 242;
constexpr Uint16 LOGO_HEIGHT = 48;

// Font position (centered)
extern volatile int fontX;
constexpr Uint16 FONT_Y = ((WINDOW_HEIGHT / 2) + FONT_HEIGHT / 2) - FONT_HEIGHT;

// Image position (centered)
constexpr Uint16 IMAGE_X = ((WINDOW_WIDTH / 2) + IMAGE_WIDTH / 2) - IMAGE_WIDTH;
constexpr Uint16 IMAGE_Y =
    (WINDOW_HEIGHT / 2) + (IMAGE_HEIGHT / 2) - IMAGE_HEIGHT;

// Logo position (bottom right) with some spacing
constexpr Uint16 LOGO_X = WINDOW_WIDTH - LOGO_WIDTH - 50;
constexpr Uint16 LOGO_Y = WINDOW_HEIGHT - LOGO_HEIGHT - 50;

constexpr int MAX_WEIGHT = 3000;

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
  SDL_Rect fontPosition;
  SDL_Rect logoPosition;
  SDL_Rect imagePosition;
  sdl_unique<SDL_Window> window;
  sdl_unique<SDL_Surface> surface;
  sdl_unique<SDL_Renderer> renderer;
  sdl_unique<SDL_Texture> logo;   // Texture for logo (always visible)
  sdl_unique<SDL_Texture> image;  // Texture for QR code
  sdl_unique<SDL_Texture> weight; // Texture for weight

  // Pointer for retrieving a font.
  sdl_unique<TTF_Font> font;

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

  // Image functions
  int loadImage();
  void presentWindow(bool font, int weight);

  // Checks
  bool checkWeight(int weight);
  int checkLengthInChar(int weight);

  void setSurfacePosition(SDL_Rect *surface, Uint16 x, Uint16 y, Uint16 w,
                          Uint16 h);
  void setFontWidth(int weight);
  void setRenderingColor(Uint8 r, Uint8 g, Uint8 b);
  void drawImageFrame();

  // Creation of needed SDL instances
  SDLError createWindow();
  SDLError createRenderer();

  // Image and Logo are SDL_Surfaces
  SDLError loadSurfaceOfIMG(const char *filepath);
  SDLError loadFontSurface(const char *filepath, int startWeight);
  SDLError updateFontSurface(const char *filepath, int newWeight);

  // Can probably be combined into one??
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

  // Frames of different images
  SDL_Rect getImageRect() const;
  SDL_Rect getLogoRect() const;
  SDL_Rect getFontRect() const;

  // Events, will not be used later
  bool getState();
  void poll();
  bool hasEvent() const;
  SDL_Event getNext();
};

#endif