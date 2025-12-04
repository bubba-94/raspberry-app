#ifndef SDLTYPES_HPP
#define SDLTYPES_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <cstdint>
#include <memory>

/**
 * @brief Structure for an SDL ptr deleter
 * @param SDL Class for example (SDL_Window or SDL_Surface)
 */
template <typename T> struct SDLDeleter;

/**
 * @brief Alias for auto deleter
 * @param T SDL Class pointer
 * @return A unique pointer of T .
 */
template <typename T> using sdl_unique = std::unique_ptr<T, SDLDeleter<T>>;

template <> struct SDLDeleter<SDL_Window> {
  SDLDeleter() = default;
  void operator()(SDL_Window *p) const { SDL_DestroyWindow(p); }
};

template <> struct SDLDeleter<SDL_Renderer> {
  SDLDeleter() = default;
  void operator()(SDL_Renderer *p) const { SDL_DestroyRenderer(p); }
};

template <> struct SDLDeleter<SDL_Texture> {
  SDLDeleter() = default;
  void operator()(SDL_Texture *p) const { SDL_DestroyTexture(p); }
};

template <> struct SDLDeleter<SDL_Surface> {
  SDLDeleter() = default;
  void operator()(SDL_Surface *p) const { SDL_FreeSurface(p); }
};

template <> struct SDLDeleter<TTF_Font> {
  SDLDeleter() = default;
  void operator()(TTF_Font *p) const { TTF_CloseFont(p); }
};

/**
 * @brief Evaluation for creating resources
 * @return NONE if successful
 * @return > 0 if not succesful
 */
enum class SDLError : std::uint8_t {
  NONE = 0,
  WINDOW_ERR = 1,
  SURFACE_ERR = 2,
  RENDER_ERR = 3,
  TEXTURE_ERR = 4,
  IMAGE_ERR = 5,
  LOGO_ERR = 6,
  FONT_ERR = 7
};

/**
 * @brief Decide the size and location of an image (SDL_Surface)
 * @param x X-coordinate for cursor
 * @param y Y-coordinate for cursor
 * @param w Length of the box in x direction
 * @param h Length of the box in y direction
 * @param xpad Length of padding in relation to the width
 * @param ypad Length of padding in relation to the height
 *
 * @return Configuration for a SDL_Surface.
 */
struct SDLImageSpec {
  Uint16 x;
  Uint16 y;
  Uint16 w;
  Uint16 h;
  Uint16 xpad;
  Uint16 ypad;
};

#endif
