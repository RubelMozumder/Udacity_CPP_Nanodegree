#include "screen_mandelbrot.h"

#include <iostream>
#include <mutex>

Screen::Screen(int screen_height, int screen_width) {
  Screen::_screen_height = screen_height;
  Screen::_screen_width = screen_width;
  Screen::_texture_height = screen_height;
  Screen::_texture_width = screen_width;
};

bool Screen::Screen_init(std::unique_ptr<char[]>&& title) {
  Screen::_title = std::move(title);
  // To check and Initialization the SDL2 is workgin perfectly
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Something with SDL initialization step has drived wrong."
              << "\n";

    return false;
  };
  // Create Window
  Screen::_window = SDL_CreateWindow(
      Screen::_title.get(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      Screen::_screen_width, Screen::_screen_height, SDL_WINDOW_SHOWN);

  // Check wether window has been created perfectly
  if (Screen::_window == NULL) {
    std::cout << "Something with SDL window has drived wrong."
              << "\n";

    SDL_DestroyWindow(Screen::_window);
    // SDL_Quit();

    return false;
  };

  // Create Renderer
  // SDL_RENDERER_PRESENTVSYNC : present is synchronized with the refresh rate
  Screen::_renderer =
      SDL_CreateRenderer(Screen::_window, -1, SDL_RENDERER_PRESENTVSYNC);

  if (Screen::_renderer == NULL) {
    std::cout << "Something drived to wrong for renderer"
              << "\n";
    std::cout << SDL_GetError() << "\n";
    SDL_DestroyWindow(Screen::_window);
    SDL_DestroyRenderer(Screen::_renderer);
    SDL_Quit();
    return false;
  }

  // Create Texture
  // SDL_PIXELFORMAT_RGBA8888 -> color format
  Screen::_texture = SDL_CreateTexture(
      Screen::_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
      Screen::_texture_height, Screen::_texture_width);

  if (Screen::_texture == NULL) {
    std::cout << "Something drived to wrong for texturer"
              << "\n";
    SDL_DestroyWindow(Screen::_window);
    SDL_DestroyRenderer(Screen::_renderer);
    SDL_DestroyTexture(Screen::_texture);
    SDL_Quit();
    return false;
  }

  return true;
};

void Screen::updatePixel() {
  if (Screen::mandelbrot.isInitialised()) {
    std::lock_guard<std::mutex> screenDataLock(Screen::_sdlMtx);
    Screen::_buffer = Screen::mandelbrot.getScreenBuffer();
  } else {
    std::cout << " Mandel Brot has not been instantiated!! "
              << "\n";
  };
}

void Screen::updateScreen() {
  SDL_UpdateTexture(Screen::_texture, NULL, Screen::_buffer.get(),
                    Screen::_texture_width * sizeof(Uint32));
  SDL_RenderClear(Screen::_renderer);
  SDL_RenderCopy(Screen::_renderer, Screen::_texture, NULL, NULL);
  SDL_RenderPresent(Screen::_renderer);
};

void Screen::Screen_destroy() {
  SDL_DestroyWindow(Screen::_window);
  SDL_Quit();
  SDL_DestroyRenderer(Screen::_renderer);
  SDL_DestroyTexture(Screen::_texture);
};

void Screen::setupMandelBrotSimulation(long double realBoudary,
                                       long double imgBoundary, int maxIter) {
  Screen::mandelbrot = MandelBrot<long double>(
      realBoudary,             // real boundary
      imgBoundary,             // Imaginary boundary
      Screen::_screen_width,   // Grid number on real axis
      Screen::_screen_height,  // Grid number on imaginary axis
      maxIter                  // Maximum allowed itetation
  );
};

void Screen::runMandelBrotSimulation() {
  Screen::mandelbrot.runMandelBrotSimulation();
};

Screen::~Screen(){};

bool Screen::processEvent() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      return true;
    };
  }
  return false;
};