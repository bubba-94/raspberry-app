#ifndef WINDOW_HPP
#define WINDOW_HPP

// SDL2 Library

// C++ standard libs
#include <SDL2/SDL.h>
#include <memory>
#include <string>

class Window {
private:
  struct DelWindow {
    void operator()(SDL_Window *ptr) const {
      if (ptr)
        SDL_DestroyWindow(ptr);
    }
  };

  std::unique_ptr<SDL_Window, DelWindow> window;
  bool state = false;

public:
  Window() = default;
  ~Window() = default;

  void createWindow(const std::string &title, int width, int height);
  bool getState();
  void shutdown();
  SDL_Window *getWindow();
};

#endif