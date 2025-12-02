#include "../include/Graphics.hpp"

SDL_Window *SDLManager::getRawWindow() const { return window.get(); }
SDL_Renderer *SDLManager::getRawRenderer() const { return renderer.get(); }
SDL_Surface *SDLManager::getRawSurface() const { return surface.get(); }
SDL_Texture *SDLManager::getRawTexture() const { return texture.get(); }

// Init calls all creations.
SDLError SDLManager::createWindow() {

  // Defaulted window specs
  window.reset(SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                WINDOW_HEIGHT, SDL_WINDOW_SHOWN));

  if (!window) {
    printErrMsg(SDL_GetError());
    return SDLError::WINDOW_ERR;
  }

  return SDLError::OK;
}

SDLError SDLManager::createRenderer() {

  renderer.reset(
      SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));

  if (!renderer) {
    printErrMsg(SDL_GetError());
    return SDLError::RENDER_ERR;
  }

  return SDLError::OK;
}

SDLError SDLManager::createSurface() {
  surface.reset(SDL_CreateRGBSurface(0,
                                     // Width and height of the surface
                                     WIDTH_PADDING, HEIGHT_PADDING,
                                     // Depth of bits
                                     32,
                                     // Color masks
                                     0, 0, 0, 0));

  if (!surface) {
    printErrMsg(SDL_GetError());
    return SDLError::SURFACE_ERR;
  }
  return SDLError::OK;
}

SDLError SDLManager::createTexture() {

  texture.reset(SDL_CreateTextureFromSurface(renderer.get(), surface.get()));

  if (!texture) {
    printErrMsg(SDL_GetError());
    return SDLError::TEXTURE_ERR;
  }
  return SDLError::OK;
}

bool SDLManager::getState() { return state; }

void SDLManager::poll() {
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent)) {
    events.push(sdlEvent);
  }
}

bool SDLManager::hasEvent() const { return !events.empty(); }

SDL_Event SDLManager::getNext() {
  if (events.empty())
    return SDL_Event{}; // return empty event if none
  SDL_Event e = events.front();
  events.pop();
  return e;
}

void SDLManager::init() {
  if (createWindow() == SDLError::OK && createRenderer() == SDLError::OK &&
      createSurface() == SDLError::OK && createTexture() == SDLError::OK) {
    state = true;
  } else
    state = false;
}

void SDLManager::shutdown() {
  std::cout << "Application being shutdown...." << "\n";
  SDL_Quit();
  std::exit(0);
}

void SDLManager::printErrMsg(const char *errMsg) {
  std::cerr << "SDL_Error  failed: " << errMsg << "\n";
}