#include "Graphics.hpp"

// Init calls all creations.
SDLError SDLManager::init() {

  // Initialize SDL library before other
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printErrMsg(SDL_GetError());
    status = false;
    return SDLError::WINDOW_ERR;
  }

  // Initialize IMG library
  int imgFlags = IMG_INIT_PNG;
  int imgStatus = IMG_Init(imgFlags);
  if ((imgStatus & imgFlags) != imgFlags) {
    status = false;
    return SDLError::IMAGE_ERR;
  }

  // Initialize TTF library
  int fontStatus = TTF_Init();
  if (fontStatus < 0) {
    printErrMsg(SDL_GetError());
    status = false;
    return SDLError::FONT_ERR;
  }

  // Initialize window, renderer, and img flags.
  if (createWindow() == SDLError::NONE && createRenderer() == SDLError::NONE) {
  }

  // Shutdown if not initialized correctly.
  if (status == false) {
    std::cout << "Initialization not correct, check errors. \n";
    shutdown();
  }

  std::cout << "SDL initialized correctly" << "\n";

  return SDLError::NONE;
}

void SDLManager::setup() {

  // Load files into memory
  loadFontSurface(FONT, 0);
  createFontTexture(getRawSurface());

  loadSurfaceOfIMG(LOGO);
  createLogoTexture(getRawSurface());

  loadSurfaceOfIMG(IMAGE);
  createImageTexture(getRawSurface());

  // Set surface framings to default
  setSurfacePosition(&qrSpec, IMAGE_X, IMAGE_Y, IMAGE_WIDTH, IMAGE_HEIGHT);
  setSurfacePosition(&logoSpec, LOGO_X, LOGO_Y, LOGO_WIDTH, LOGO_HEIGHT);
  setSurfacePosition(&weightSpec, fontX, FONT_Y, fontWidth, FONT_HEIGHT);
}

void SDLManager::render(int newWeight) {
  SDL_RenderClear(getRawRenderer());
  bool check = checkWeight(newWeight);

  // Proceed if check valid and needs update
  if (check == true) {
    std::cout << "Current weight: " << newWeight << "\n";
    updateFontSurface(newWeight);
    createFontTexture(getRawSurface());
  }

  // Switch the rendering QR, or WEIGHT
  if (showImage) {
    SDL_RenderCopy(getRawRenderer(), getRawWeight(), NULL, &weightSpec.rect);
  } else {
    SDL_RenderCopy(getRawRenderer(), getRawImage(), NULL, &qrSpec.rect);
  }

  // Always present logo
  SDL_RenderCopy(getRawRenderer(), getRawLogo(), NULL, &logoSpec.rect);

  SDL_RenderPresent(getRawRenderer());
  SDL_Delay(16);
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
#ifdef RPI
  window.reset(SDL_CreateWindow(
      WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT,
      SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN));
#else
  window.reset(SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                WINDOW_HEIGHT, SDL_WINDOW_SHOWN));
#endif

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

  // Optimize the qr code.
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

  font.reset(TTF_OpenFont(filepath, 400));

  if (!font) {
    printErrMsg(SDL_GetError());
    return SDLError::FONT_ERR;
  }

  std::string text = std::to_string(weight);
  const char *weightConverted = text.c_str();

  surface.reset(
      TTF_RenderUTF8_Solid(getRawFont(), weightConverted, weightSpec.color));

  if (!surface) {
    printErrMsg(SDL_GetError());
    return SDLError::SURFACE_ERR;
  }
  return SDLError::NONE;
}

SDLError SDLManager::updateFontSurface(int newWeight) {

  std::string text = std::to_string(newWeight);
  const char *weightConverted = text.c_str();

  setFontWidth(newWeight);
  setSurfacePosition(&weightSpec, fontX, FONT_Y, fontWidth, FONT_HEIGHT);

  surface.reset(
      TTF_RenderUTF8_Blended(getRawFont(), weightConverted, weightSpec.color));

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

bool SDLManager::getStatus() { return status; }

bool SDLManager::hasEvent() const { return !events.empty(); }

// Event functions
void SDLManager::poll(bool state) { showImage = state; }

void SDLManager::pollEvents() {

  while (SDL_PollEvent(&event)) {

    events.push(event);

    while (hasEvent()) {

      event = getNext();

      switch (event.type) {

      case SDL_QUIT:
        std::cout << "Closing SDL Window " << '\n';
        shutdown();
        break;

      case SDL_KEYDOWN:
        showImage = !showImage;
        std::cout << "Key pressed: " << SDL_GetKeyName(event.key.keysym.sym)
                  << '\n';
        break;

      case SDL_MOUSEBUTTONDOWN:
        std::cout << "Switching texture: \n";
        showImage = !showImage;
        break;
      default:
        break;
      }
    }
  }
}

SDL_Event SDLManager::getNext() {
  if (events.empty())
    return SDL_Event{}; // return empty event if none
  event = events.front();
  events.pop();
  return event;
}

void SDLManager::setRenderingColor(Uint8 r, Uint8 g, Uint8 b) {
  SDL_RenderClear(getRawRenderer());
  // Set a white window
  SDL_SetRenderDrawColor(getRawRenderer(), r, g, b, SDL_ALPHA_OPAQUE);
}

void SDLManager::setSurfacePosition(SDLSpec *surface, Uint16 x, Uint16 y,
                                    Uint16 w, Uint16 h) {
  // Standard white color
  surface->color.a = 255;
  surface->color.r = 255;
  surface->color.b = 255;
  surface->color.g = 255;

  surface->rect.x = x;
  surface->rect.y = y;
  surface->rect.w = w;
  surface->rect.h = h;
}

void SDLManager::setFontWidth(int weight) {
  int length = checkLengthInChar(weight);

  fontWidth = FONT_CHAR_SIZE * length;

  fontX = ((WINDOW_WIDTH / 2) + fontWidth / 2) - fontWidth;

  setSurfacePosition(&weightSpec, fontX, FONT_Y, fontWidth, FONT_HEIGHT);
}

SDL_Window *SDLManager::getRawWindow() const { return window.get(); }
SDL_Renderer *SDLManager::getRawRenderer() const { return renderer.get(); }
SDL_Surface *SDLManager::getRawSurface() const { return surface.get(); }
SDL_Texture *SDLManager::getRawImage() const { return image.get(); }
SDL_Texture *SDLManager::getRawLogo() const { return logo.get(); }
SDL_Texture *SDLManager::getRawWeight() const { return weight.get(); }
TTF_Font *SDLManager::getRawFont() const { return font.get(); }
