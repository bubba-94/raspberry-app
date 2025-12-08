#include "Graphics.hpp"

volatile int fontWidth = 250;
volatile int fontX = ((WINDOW_WIDTH / 2) + fontWidth / 2) - fontWidth;

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

  // Load files into memory
  loadFontSurface(FONT, 0);
  createFontTexture(getRawSurface());

  loadSurfaceOfIMG(LOGO);
  createLogoTexture(getRawSurface());

  loadSurfaceOfIMG(IMAGE);
  createImageTexture(getRawSurface());

  // Set surface framings to default
  setSurfacePosition(&imagePosition, IMAGE_X, IMAGE_Y, IMAGE_WIDTH,
                     IMAGE_HEIGHT);

  setSurfacePosition(&logoPosition, LOGO_X, LOGO_Y, LOGO_WIDTH, LOGO_HEIGHT);
  setSurfacePosition(&fontPosition, fontX, FONT_Y, fontWidth, FONT_HEIGHT);

  // Shutdown if not initialized correctly.
  if (state == false) {
    std::cout << "Initialization not correct, check errors. \n";
    shutdown();
  }

  std::cout << "SDL initialized correctly" << "\n";

  return SDLError::NONE;
}

void SDLManager::presentWindow(bool font, int newWeight) {

  setRenderingColor(0, 0, 0);
  // Check if weight in realation to previous weight
  bool check = checkWeight(newWeight);

  // Proceed if check valid and needs update
  if (check == true) {
    std::cout << "Current weight: " << newWeight << "\n";
    updateFontSurface(FONT, newWeight);
    createFontTexture(getRawSurface());
  }

  // Update rendering
  if (font) {
    SDL_RenderCopy(getRawRenderer(), getRawWeight(), NULL, &fontPosition);
  } else {
    SDL_RenderCopy(getRawRenderer(), getRawImage(), NULL, &imagePosition);
  }

  // Always present logo
  SDL_RenderCopy(getRawRenderer(), getRawLogo(), NULL, &logoPosition);

  SDL_RenderPresent(getRawRenderer());
}

void SDLManager::shutdown() {
  std::cout << "Application being shutdown...." << "\n";

  // End other libraries before SDL Library
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
  /*
   *
   weight.reset(SDL_CreateTexture(
    getRawRenderer(),
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_STREAMING, // Allows direct pixel updates
    FONT_WIDTH,
    FONT_HEIGHT
  ));
  */

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

SDLError SDLManager::loadFontSurface(const char *filepath, int weight) {

  SDL_Color color{255, 255, 255, 255};

  font.reset(TTF_OpenFont(filepath, 400));

  if (!font) {
    printErrMsg(SDL_GetError());
    return SDLError::FONT_ERR;
  }

  std::string text = std::to_string(weight);
  const char *weightConverted = text.c_str();

  surface.reset(TTF_RenderUTF8_Blended(getRawFont(), weightConverted, color));

  if (!surface) {
    printErrMsg(SDL_GetError());
    return SDLError::SURFACE_ERR;
  }
  return SDLError::NONE;
}

SDLError SDLManager::updateFontSurface(const char *filepath, int newWeight) {

  SDL_Color color{255, 255, 255, 255};

  font.reset(TTF_OpenFont(filepath, 400));

  if (!font) {
    printErrMsg(SDL_GetError());
    return SDLError::FONT_ERR;
  }

  std::string text = std::to_string(newWeight);
  const char *weightConverted = text.c_str();

  setFontWidth(newWeight);
  setSurfacePosition(&fontPosition, fontX, FONT_Y, fontWidth, FONT_HEIGHT);

  surface.reset(TTF_RenderUTF8_Blended(getRawFont(), weightConverted, color));

  if (!surface) {
    printErrMsg(SDL_GetError());
    return SDLError::SURFACE_ERR;
  }
  return SDLError::NONE;
}

bool SDLManager::checkWeight(int weight) {
  // Weight gets incremented in main loop
  // Set when parameters match
  static int previousWeight{};

  if (weight > MAX_WEIGHT) {
    return false;
  }

  if (weight != previousWeight) {
    previousWeight = weight;
    return true;
  }

  previousWeight = weight;
  return false;
}

int SDLManager::checkLengthInChar(int weight) {

  std::string value = std::to_string(weight);
  Uint8 length = value.length();

  return length;
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
SDL_Rect SDLManager::getImageRect() const { return this->imagePosition; }
SDL_Rect SDLManager::getLogoRect() const { return this->logoPosition; }
SDL_Rect SDLManager::getFontRect() const { return this->fontPosition; }

void SDLManager::setRenderingColor(Uint8 r, Uint8 g, Uint8 b) {
  SDL_RenderClear(getRawRenderer());
  // Set a white window
  SDL_SetRenderDrawColor(getRawRenderer(), r, g, b, SDL_ALPHA_OPAQUE);
}

void SDLManager::setSurfacePosition(SDL_Rect *surface, Uint16 x, Uint16 y,
                                    Uint16 w, Uint16 h) {
  surface->x = x;
  surface->y = y;
  surface->w = w;
  surface->h = h;
}

void SDLManager::setFontWidth(int weight) {
  int length = checkLengthInChar(weight);

  // Change width and x cursor of font whenever every thousand integer value
  if (length == 1) {
    fontWidth = 250;
    setSurfacePosition(&fontPosition, fontX, FONT_Y, fontWidth, FONT_HEIGHT);
  }
  if (length == 2) {
    fontWidth = 250 * 2;
    fontX = ((WINDOW_WIDTH / 2) + fontWidth / 2) - fontWidth;
    setSurfacePosition(&fontPosition, fontX, FONT_Y, fontWidth, FONT_HEIGHT);
  }
  if (length == 3) {
    fontWidth = 250 * 3;
    fontX = ((WINDOW_WIDTH / 2) + fontWidth / 2) - fontWidth;
    setSurfacePosition(&fontPosition, fontX, FONT_Y, fontWidth, FONT_HEIGHT);
  }
  if (length == 4) {
    fontWidth = 250 * 4;
    fontX = ((WINDOW_WIDTH / 2) + fontWidth / 2) - fontWidth;
    setSurfacePosition(&fontPosition, fontX, FONT_Y, fontWidth, FONT_HEIGHT);
  }
}