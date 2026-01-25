#include "Graphics.hpp"

SDLManager::SDLManager(const std::string &windowTitle) {
  // Init SDL
  std::cout << "[SDL] Start initialization" << "\n";
  if (SDL_Init(SDL_INIT_VIDEO < 0))
    printErrMsg(SDL_GetError());
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    printErrMsg(SDL_GetError());
  if (TTF_Init() < 0)
    printErrMsg(SDL_GetError());

  int windowFlags = SDL_WINDOW_SHOWN;
#ifdef RPI
  windowFlags |= (SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN);
#endif

  // Create window from specifics
  window.reset(SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                WINDOW_HEIGHT, windowFlags));

  int renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  if (!window)
    printErrMsg(SDL_GetError());

  // Assign renderer to window
  renderer.reset(SDL_CreateRenderer(getRawWindow(), -1, renderFlags));

  // Try fallback first
  if (!renderer)
    renderer.reset(
        SDL_CreateRenderer(getRawWindow(), -1, SDL_RENDERER_SOFTWARE));
  if (!renderer)
    printErrMsg(SDL_GetError());

  // State of window
  status = true;

  setup();
  std::cout << "[SDL] Initialization successful" << "\n";
}
SDLManager::~SDLManager() {
  std::cout << "[SDL] Application being shutdown...." << "\n";

  // End other libraries before SDL Library
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  std::exit(1);
}

void SDLManager::setup() {

  // Set surface framings to default
  createTextures();

  setSurfacePosition(&qrSpec, IMAGE_X, IMAGE_Y, IMAGE_WIDTH, IMAGE_HEIGHT);
  setSurfacePosition(&logoSpec, LOGO_X, LOGO_Y, LOGO_WIDTH, LOGO_HEIGHT);
  setSurfacePosition(&weightSpec, weightX, WEIGHT_Y, weightWidth, WEIGHT_HEIGHT);
}

void SDLManager::render(int newWeight) {
  SDL_RenderClear(getRawRenderer());
  bool check = checkWeight(newWeight);

  // Proceed if check valid and needs update
  if (check == true) {
    std::cout << "[SDL] New weight: " << newWeight << "\n";
    updateFontTexture(newWeight);
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

void SDLManager::printErrMsg(const char *errMsg) {
  std::cerr << "SDL_Error occured: " << errMsg << "\n";
}

void SDLManager::createTextures() {
  loadSurfaceOfIMG(LOGO);
  logo.reset(SDL_CreateTextureFromSurface(getRawRenderer(), getRawSurface()));
  if (!logo)
    printErrMsg(SDL_GetError());

  loadSurfaceOfIMG(IMAGE);
  image.reset(SDL_CreateTextureFromSurface(getRawRenderer(), getRawSurface()));
  if (!image)
    printErrMsg(SDL_GetError());

  loadFontSurface(FONT);
  weight.reset(SDL_CreateTextureFromSurface(getRawRenderer(), getRawSurface()));
  if (!weight)
    printErrMsg(SDL_GetError());
}

// Imaging
void SDLManager::loadSurfaceOfIMG(const char *filepath) {
  surface.reset(IMG_Load(filepath));

  if (!surface)
    printErrMsg(SDL_GetError());
}

void SDLManager::loadFontSurface(const char *filepath) {

  font.reset(TTF_OpenFont(filepath, 400));

  if (!font)
    printErrMsg(SDL_GetError());

  const char *value = "0";

  surface.reset(
      TTF_RenderUTF8_Solid(getRawFont(), value, weightSpec.color));

  if (!surface)
    printErrMsg(SDL_GetError());
}

void SDLManager::updateFontTexture(int newWeight) {

  std::string text = std::to_string(newWeight);
  const char *weightConverted = text.c_str();

  setFontWidth(newWeight);
  setSurfacePosition(&weightSpec, weightX, WEIGHT_Y, weightWidth, WEIGHT_HEIGHT);

  surface.reset(
      TTF_RenderUTF8_Blended(getRawFont(), weightConverted, weightSpec.color));

  if (!surface)
    printErrMsg(SDL_GetError());

  weight.reset(SDL_CreateTextureFromSurface(getRawRenderer(), getRawSurface()));

  if (!weight)
    printErrMsg(SDL_GetError());
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

bool SDLManager::getStatus() { return status; }

bool SDLManager::hasEvent() const { return !events.empty(); }

#ifdef RPI
void SDLManager::poll(const PinState &state) {

  // Key switches the image shown
  showImage = state.keyEnabled;

  // If button is pressed, shutdown
  if (state.shutdownRequested) {
    status = false;
  }
}
#endif

void SDLManager::pollEvents() {

  while (SDL_PollEvent(&event)) {

    events.push(event);

    while (hasEvent()) {

      event = getNext();

      switch (event.type) {

      case SDL_QUIT:
        std::cout << "[SDL] Closing SDL Window " << '\n';
        status = false;
        break;

      case SDL_KEYDOWN:
        showImage = !showImage;
        std::cout << "[SDL] Key pressed: "
                  << SDL_GetKeyName(event.key.keysym.sym) << '\n';
        break;

      case SDL_MOUSEBUTTONDOWN:
        std::cout << "[SDL] Switching texture: \n";
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

int SDLManager::checkLengthOfWeight(int weight) {

  std::string value = std::to_string(weight);
  Uint8 length = value.length();

  return length;
}

void SDLManager::setFontWidth(int weight) {
  int length = checkLengthOfWeight(weight);

  weightWidth = WEIGHT_CHAR_SIZE * length;

  weightX = ((WINDOW_WIDTH / 2) + weightWidth / 2) - weightWidth;

  setSurfacePosition(&weightSpec, weightX, WEIGHT_Y, weightWidth, WEIGHT_HEIGHT);
}

SDL_Window *SDLManager::getRawWindow() const { return window.get(); }
SDL_Renderer *SDLManager::getRawRenderer() const { return renderer.get(); }
SDL_Surface *SDLManager::getRawSurface() const { return surface.get(); }
SDL_Texture *SDLManager::getRawImage() const { return image.get(); }
SDL_Texture *SDLManager::getRawLogo() const { return logo.get(); }
SDL_Texture *SDLManager::getRawWeight() const { return weight.get(); }
TTF_Font *SDLManager::getRawFont() const { return font.get(); }
