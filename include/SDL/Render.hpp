#ifndef RENDERER_HPP
#define RENDERER_HPP

// Linked libs
#include <SDL2/SDL_render.h>

// C++ standard libs
#include <memory>

// Forward declares, maybe not needed
class Window;

class Renderer {
private:
  /// @brief  Auto deleter when renderer goes out of scope
  struct DelRenderer {
    void operator()(SDL_Renderer *ptr) const {
      if (ptr)
        SDL_DestroyRenderer(ptr);
    }
  };

  /// @brief Pointer owned by renderer
  std::unique_ptr<SDL_Renderer, DelRenderer> renderer;
  bool state = false;

public:
  Renderer(Window &window);
  ~Renderer() = default;
  void createRenderer(Window *window);
};

#endif