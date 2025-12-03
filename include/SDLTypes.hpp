#ifndef SDLTYPES_HPP
#define SDLTYPES_HPP

#include <SDL2/SDL.h>

#include <cstdint>
#include <memory>

/**
 * @brief Structure for an SDL ptr deleter
 * @param SDL Class for example (SDL_Window or SDL_Surface)
 */
template <typename T> struct SDL_Deleter;

/**
 * @brief Alias for auto deleter
 * @param T SDL Class pointer (SDL_Window)
 * @return A unique pointer of T .
 */
template <typename T> using sdl_unique = std::unique_ptr<T, SDL_Deleter<T>>;

template <> struct SDL_Deleter<SDL_Window> {
  SDL_Deleter() = default;
  void operator()(SDL_Window *p) const { SDL_DestroyWindow(p); }
};

template <> struct SDL_Deleter<SDL_Renderer> {
  SDL_Deleter() = default;
  void operator()(SDL_Renderer *p) const { SDL_DestroyRenderer(p); }
};

template <> struct SDL_Deleter<SDL_Texture> {
  SDL_Deleter() = default;
  void operator()(SDL_Texture *p) const { SDL_DestroyTexture(p); }
};

template <> struct SDL_Deleter<SDL_Surface> {
  SDL_Deleter() = default;
  void operator()(SDL_Surface *p) const { SDL_FreeSurface(p); }
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
  LOGO_ERR = 6
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
  int x;
  int y;
  int w;
  int h;
  int xpad;
  int ypad;
};

#endif
