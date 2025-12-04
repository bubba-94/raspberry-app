#include "Graphics.hpp"

// Init calls all creations.
SDLError SDLManager::init() {

  // Initialize SDL library before other
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printErrMsg(SDL_GetError());
    state = false;
    return SDLError::WINDOW_ERR;
  }

  // Initialize IMG library
  int imgFlags = IMG_INIT_PNG;
  int imgStatus = IMG_Init(imgFlags);
  if ((imgStatus & imgFlags) != imgFlags) {
    state = false;
    return SDLError::IMAGE_ERR;
  }

  // Initialize TTF library
  int fontStatus = TTF_Init();
  if (fontStatus < 0) {
    printErrMsg(SDL_GetError());
    state = false;
    return SDLError::FONT_ERR;
  }

  // Initialize window, renderer, and img flags.
  if (createWindow() == SDLError::NONE && createRenderer() == SDLError::NONE) {
  }

  // Load logo into memory
  loadSurfaceOfIMG(LOGO);
  createLogoTexture(getRawSurface());

  // Load qr image into  memory
  loadSurfaceOfIMG(IMAGE);
  createImageTexture(getRawSurface());

  // Load font into memory
  loadSurfaceOfFont(FONT, TEST_WEIGHT);
  createFontTexture(getRawSurface());

  // Shutdown if not initialized correctly.
  if (state == false) {
    std::cout << "Initialization not correct, check errors. \n";
    shutdown();
  }

  std::cout << "SDL initialized correctly" << "\n";

  return SDLError::NONE;
}

void SDLManager::presentWindow(bool font) {

  setRenderingColor(0, 0, 0);

  // Set the cursor and sizes of both surfaces.
  SDL_Rect logoFrame, imageFrame, fontFrame;
  logoFrame = {LOGO_X, LOGO_Y, LOGO_WIDTH, LOGO_HEIGHT};
  fontFrame = {FONT_X, FONT_Y, FONT_WIDTH, FONT_HEIGHT};
  imageFrame = {IMAGE_X, IMAGE_Y, IMAGE_WIDTH, IMAGE_HEIGHT};

  // Always show logo

  // If parameter is true, show number else show QR image
  if (font) {
    SDL_RenderCopy(getRawRenderer(), getRawWeight(), NULL, &fontFrame);
  } else {
    SDL_RenderCopy(getRawRenderer(), getRawImage(), NULL, &imageFrame);
  }
  SDL_RenderCopy(getRawRenderer(), getRawLogo(), NULL, &logoFrame);

  SDL_RenderPresent(getRawRenderer());
}

void SDLManager::shutdown() {
  std::cout << "Application being shutdown...." << "\n";

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  std::exit(1);
}

void SDLManager::printErrMsg(const char *errMsg) {
  std::cerr << "SDL_Error occured: " << errMsg << "\n";
}

SDLError SDLManager::createWindow() {
  // Defaulted window specs
  window.reset(SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                WINDOW_HEIGHT, SDL_WINDOW_SHOWN));

  if (!window) {
    printErrMsg(SDL_GetError());
    return SDLError::WINDOW_ERR;
  }

  return SDLError::NONE;
}

SDLError SDLManager::createRenderer() {
  // Assign renderer to window. Will only have one window open.

  int flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

  renderer.reset(SDL_CreateRenderer(getRawWindow(), -1, flags));

  if (!renderer) {
    renderer.reset(
        SDL_CreateRenderer(getRawWindow(), -1, SDL_RENDERER_SOFTWARE));
  }

  if (!renderer) {
    printErrMsg(SDL_GetError());
    return SDLError::RENDER_ERR;
  }

  return SDLError::NONE;
}

SDLError SDLManager::createImageTexture(SDL_Surface *surface) {

  // Create a texture from surface (image or logo)
  image.reset(SDL_CreateTextureFromSurface(getRawRenderer(), surface));

  if (!image) {
    printErrMsg(SDL_GetError());
    return SDLError::IMAGE_ERR;
  }

  // Optimized the qr code.
  SDL_SetTextureScaleMode(getRawImage(), SDL_ScaleModeNearest);

  return SDLError::NONE;
}

SDLError SDLManager::createLogoTexture(SDL_Surface *surface) {

  // Create a texture from surface (image or logo)
  logo.reset(SDL_CreateTextureFromSurface(getRawRenderer(), surface));

  if (!logo) {
    printErrMsg(SDL_GetError());
    return SDLError::LOGO_ERR;
  }

  return SDLError::NONE;
}

SDLError SDLManager::createFontTexture(SDL_Surface *surface) {

  // Texture of a font surface
  weight.reset(SDL_CreateTextureFromSurface(getRawRenderer(), surface));

  if (!weight) {
    printErrMsg(SDL_GetError());
    return SDLError::FONT_ERR;
  }

  return SDLError::NONE;
}

// Imaging
SDLError SDLManager::loadSurfaceOfIMG(const char *filepath) {
  surface.reset(IMG_Load(filepath));

  if (!surface) {
    printErrMsg(SDL_GetError());
    return SDLError::SURFACE_ERR;
  }
  return SDLError::NONE;
}

SDLError SDLManager::loadSurfaceOfFont(const char *filepath, int weight) {

  SDL_Color color{255, 255, 255, 255};

  font.reset(TTF_OpenFont(filepath, 400));

  if (!font) {
    printErrMsg(SDL_GetError());
    return SDLError::FONT_ERR;
  }

  std::string text = std::to_string(weight);
  const char *weightConverted = text.c_str();

  surface.reset(TTF_RenderUTF8_Solid(getRawFont(), weightConverted, color));

  if (!surface) {
    printErrMsg(SDL_GetError());
    return SDLError::SURFACE_ERR;
  }
  return SDLError::NONE;
}

bool SDLManager::getState() { return state; }

bool SDLManager::hasEvent() const { return !events.empty(); }

// Event functions
void SDLManager::poll() {
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent)) {
    events.push(sdlEvent);
  }
}

SDL_Event SDLManager::getNext() {
  if (events.empty())
    return SDL_Event{}; // return empty event if none
  SDL_Event e = events.front();
  events.pop();
  return e;
}

SDL_Window *SDLManager::getRawWindow() const { return window.get(); }
SDL_Renderer *SDLManager::getRawRenderer() const { return renderer.get(); }
SDL_Surface *SDLManager::getRawSurface() const { return surface.get(); }
SDL_Texture *SDLManager::getRawImage() const { return image.get(); }
SDL_Texture *SDLManager::getRawLogo() const { return logo.get(); }
SDL_Texture *SDLManager::getRawWeight() const { return weight.get(); }
TTF_Font *SDLManager::getRawFont() const { return font.get(); }

void SDLManager::setRenderingColor(Uint8 r, Uint8 g, Uint8 b) {
  SDL_RenderClear(getRawRenderer());
  // Set a white window
  SDL_SetRenderDrawColor(getRawRenderer(), r, g, b, SDL_ALPHA_OPAQUE);
}
