#ifndef SDLTYPES_HPP
#define SDLTYPES_HPP

#include <SDL2/SDL.h>

#include <cstdint>
#include <memory>

/// @brief Structure for an SDL ptr deleter
/// @param SDL Class for example (SDL_Renderer, SDL_Window, SDL_Texture,
/// SDL_Surface.)
template <typename T> struct SDL_Deleter;

/// @brief Alias for auto deleter
/// @param T SDL Class pointer (SDL_Window)
/// @return A unique pointer of T .
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

enum class SDLError : std::uint8_t {
  OK = 0,
  WINDOW_ERR = 1,
  RENDER_ERR = 2,
  SURFACE_ERR = 3,
  TEXTURE_ERR = 4
};

#endif
