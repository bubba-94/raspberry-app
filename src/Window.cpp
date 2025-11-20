#include "Window.hpp"

void Window::createWindow(const std::string &title, int width, int height) {
  window.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, width, height,
                                SDL_WINDOW_SHOWN));
  state = true;
}

bool Window::getState() { return state; }

void Window::shutdown() {

  // Destroy the window
  window.reset();
  state = false;
  SDL_Quit();

  // Terminate terminal process
  std::exit(0);
}

SDL_Window *Window::getWindow() { return window.get(); }