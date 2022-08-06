#ifndef _MANDELBROT_H
#define _MANDELBROT_H

#include <complex>
#include <mutex>
#include <vector>

template <typename T1, typename T2>
struct MandelData {
  MandelData(std::complex<T1> comPoint, T2 iterNumber)
      : comPoint(comPoint), iterNumber(iterNumber){};

  std::complex<T1> comPoint;
  T2 iterNumber = 0;
};

template <class T>
class MandelBrot {
 public:
  MandelBrot();
  MandelBrot(T realBoundary, T imgBoundary, int realGridNumber,
             int imgGridNumber, int maxIter);

  // copy constructor
  MandelBrot(const MandelBrot<T> &mandelbrot_);
  // copy assignment operator
  MandelBrot<T> &operator=(const MandelBrot<T> &source_obj);

  // zoomMandelBrot()
  void constructComplexSpace();
  void resetMandelBrotParameters(T realBoundary, T imgBoundary,
                                 int realGridNumber, int imgGridNumber,
                                 int maxIter);
  int searchMandelBrotSetPoint(std::complex<T> c);
  // TODO: constructMandelBrotSync() delete this function
  void constructMandelBrotSync();
  void constructMandelBrot();

  void runMandelBrotSimulation();

  std::shared_ptr<Uint32[]> getScreenBuffer();
  // TODO: checkDataPoints delete this funcion;
  void checkDataPoints();
  bool isInitialised() { return _isInitialised; };

 private:
  bool _isInitialised{false};
  std::vector<MandelData<T, int>> _pointSpace;
  T _absZnBoundary{2.0};
  T _realBoundary;
  T _imgBoundary;
  int _realGridNumber{600};
  int _imgGridNumber{600};
  T _realGridSize;
  T _imgGridSize;
  int _maxIter;
  std::mutex _mandelMtx;
  std::shared_ptr<Uint32[]> _buffer = NULL;
};

#include "mandelbrot_empl.h"

#endif