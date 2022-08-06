#include <iostream>

//#include "mandelbrot.h"
#include "screen_mandelbrot.h"

using namespace std;

int main() {
  char tit[] = "Mandel Brot Window";
  std::unique_ptr<char[]> title(new char[strlen(tit)]);
  for (std::size_t i = 0; i < (strlen(tit)); i++) {
    title[0] = tit[i];
  };

  long double relMax = 2.5;
  long double imgMax = 2.5;
  int xGridNum = 600;
  int yGridNum = 600;
  // int ZnBoundary = 2.0;
  int iterMax = 700;

  Screen scrn(xGridNum, yGridNum);

  // TODO: Run Other steps here:
  scrn.setupMandelBrotSimulation(relMax, imgMax, iterMax);
  scrn.runMandelBrotSimulation();
  scrn.updatePixel();

  if (scrn.Screen_init(std::move(title))) {
    std::cout << "Everything is fine scree initialization : "
              << "\n";
  };

  while (true) {
    if (scrn.processEvent()) {
      break;
    };
    // std::this_thread::sleep_for(std::chrono::microseconds(100));
    scrn.updateScreen();
  };

  scrn.Screen_destroy();

  scrn.Screen_destroy();
  return 0;
};