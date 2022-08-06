#ifndef _SCREEN_MANDELBROT_H
#define _SCREEN_MANDELBROT_H
#include <SDL2/SDL.h>

#include <memory>
#include <mutex>
#include <vector>

#include "mandelbrot.h"
// TODO: try to replace "long double" in
// MandelBrot<long double> mandelbrot; by template T
class Screen {
 public:
  Screen(int screen_height, int screen_width);

  bool Screen_init(std::unique_ptr<char[]>&& title);
  bool processEvent();
  void updatePixel();
  void updateScreen();
  void Screen_destroy();
  ~Screen();

  // TODO try to instantiate MandelBrot object
  // Public Member Variable;
  MandelBrot<long double> mandelbrot;
  void setupMandelBrotSimulation(long double realBoudary,
                                 long double imgBoundary, int maxIter);
  void runMandelBrotSimulation();

 private:
  int _screen_height;
  int _screen_width;
  int _texture_height;
  int _texture_width;

  SDL_Window* _window;
  SDL_Renderer* _renderer;
  SDL_Texture* _texture;
  std::shared_ptr<Uint32[]> _buffer = NULL;
  std::unique_ptr<char[]> _title;

  std::mutex _sdlMtx;
};

#endif