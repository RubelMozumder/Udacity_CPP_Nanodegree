#include <SDL2/SDL.h>
#include <math.h>

#include <complex>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "mandelbrot.h"

// Default constructor
template <class T>
MandelBrot<T>::MandelBrot(){};

// constructor
template <class T>
MandelBrot<T>::MandelBrot(T realBoundary, T imgBoundary, int realGridNumber,
                          int imgGridNumber, int maxIter) {
  _realBoundary = realBoundary;
  _imgBoundary = imgBoundary;
  _realGridNumber = realGridNumber;
  _imgGridNumber = imgGridNumber;
  _maxIter = maxIter;
  // add data validation for given data;
  constructComplexSpace();
  _isInitialised = true;
};

// Copy constructor
template <class T>
MandelBrot<T>::MandelBrot(const MandelBrot<T> &mandelbrot_) {
  this->_realBoundary = mandelbrot_._realBoundary;
  this->_imgBoundary = mandelbrot_._imgBoundary;
  this->_realGridNumber = mandelbrot_._realGridNumber;
  this->_imgGridNumber = mandelbrot_._imgGridNumber;
  this->_maxIter = mandelbrot_._maxIter;
  this->_isInitialised = mandelbrot_._isInitialised;
  this->constructComplexSpace();
};

// Copy Assignment operator
template <class T>
MandelBrot<T> &MandelBrot<T>::operator=(const MandelBrot<T> &source_obj) {
  // Copy assignment operator
  std::cout << "Debug: Assigning content of instance " << &source_obj
            << " to instance " << this << "\n";
  if (this == &source_obj) {
    return *this;
  }

  this->_realBoundary = source_obj._realBoundary;
  this->_imgBoundary = source_obj._imgBoundary;
  this->_realGridNumber = source_obj._realGridNumber;
  this->_imgGridNumber = source_obj._imgGridNumber;
  this->_maxIter = source_obj._maxIter;
  this->_isInitialised = source_obj._isInitialised;

  this->constructComplexSpace();

  return *this;
};

template <class T>
void MandelBrot<T>::constructComplexSpace() {
  MandelBrot<T>::_realGridSize = (1.0 + MandelBrot<T>::_realBoundary * 2.0) /
                                 (MandelBrot<T>::_realGridNumber);
  MandelBrot::_imgGridSize =
      (1.0 + MandelBrot<T>::_imgBoundary * 2.0) / MandelBrot<T>::_imgGridNumber;

  T xLeft = -MandelBrot<T>::_realBoundary;
  T yDown = -MandelBrot<T>::_imgBoundary;

  for (auto row = 0; row < MandelBrot<T>::_imgGridNumber; row++) {
    T y = yDown + row * MandelBrot<T>::_imgGridSize;
    for (auto col = 0; col < MandelBrot<T>::_realGridNumber; col++) {
      T x = xLeft + col * MandelBrot<T>::_realGridSize;
      // index = [row * xGridSteps + col]

      MandelBrot<T>::_pointSpace.emplace_back(
          MandelData<T, int>(std::complex<T>(x, y), 0));
    };
  };
};
template <class T>
void MandelBrot<T>::resetMandelBrotParameters(T realBoundary, T imgBoundary,
                                              int realGridNumber,
                                              int imgGridNumber, int maxIter) {
  _realBoundary = realBoundary;
  _imgBoundary = imgBoundary;
  _realGridNumber = realGridNumber;
  _imgGridNumber = imgGridNumber;
  _maxIter = maxIter;
  // add data validation for given data;
  constructComplexSpace();
  _isInitialised = true;
};

template <class T>
int MandelBrot<T>::searchMandelBrotSetPoint(std::complex<T> c) {
  int iter = 0;
  std::complex<T> Zn(0, 0);
  std::complex<T> Zn1;
  while (iter < MandelBrot<T>::_maxIter) {
    iter++;
    Zn1 = Zn * Zn + c;

    if (std::abs(Zn1) >= MandelBrot<T>::_absZnBoundary) {
      return iter;
    }
    Zn = Zn1;
  }
  return iter;
};

template <class T>
void MandelBrot<T>::constructMandelBrotSync() {
  for (MandelData<T, int> &mandelData : MandelBrot<T>::_pointSpace) {
    int iterNumberWithComlxC = searchMandelBrotSetPoint(mandelData.comPoint);
    mandelData.iterNumber = iterNumberWithComlxC;
  }
};
template <class T>
void MandelBrot<T>::constructMandelBrot() {
  std::vector<std::future<int>> ftrs;
  // TODO: Try and check what happens when we use `&mandelData`
  // instead of `mandelData`
  {
    std::lock_guard<std::mutex> mlock(MandelBrot<T>::_mandelMtx);
    for (MandelData<T, int> mandelData : MandelBrot<T>::_pointSpace) {
      ftrs.emplace_back(std::async(std::launch::deferred,
                                   &MandelBrot<T>::searchMandelBrotSetPoint,
                                   this, mandelData.comPoint));
    };
  }

  for (auto &ftr : ftrs) {
    ftr.wait();
  };
  {
    std::lock_guard<std::mutex> mlock(MandelBrot<T>::_mandelMtx);
    for (std::size_t i = 0; i < ftrs.size(); i++) {
      MandelBrot<T>::_pointSpace[i].iterNumber = ftrs[i].get();
    }
  };
};

template <class T>
void MandelBrot<T>::checkDataPoints() {
  for (auto &mandelData : MandelBrot<T>::_pointSpace) {
    std::cout << "DC: real " << mandelData.comPoint.real() << " DC: img "
              << mandelData.comPoint.imag()
              << " iter : " << mandelData.iterNumber << "\n";
  };
}

template <class T>
void MandelBrot<T>::runMandelBrotSimulation() {
  MandelBrot<T>::constructComplexSpace();
  MandelBrot<T>::constructMandelBrot();
};

template <class T>
std::shared_ptr<Uint32[]> MandelBrot<T>::getScreenBuffer() {
  int buffer_size =
      MandelBrot<T>::_imgGridNumber * MandelBrot<T>::_realGridNumber;
  Uint32 *buf = new Uint32[buffer_size];
  int iterMax = MandelBrot<T>::_maxIter;
  double colrUnit = 255.0 / iterMax;
  int iter;
  unsigned int pixCol;
  int green_sharp = 1;
  int color_scale;

  // Color distribution of colr: 0<x<1
  //==================================
  // TODO: edit the formula
  // Color = R * 1 + G * (exp(1/a(1/2 - x)**2)) / exp(1)  + B (1-x)
  // G = R = B = 255, height value of G, R and B.

  for (int i = 0; i < buffer_size; i++) {
    // Here Converting the iteration for Mandel Set to 0
    // out of Mandel Set with high number
    iter = std::abs(MandelBrot<T>::_pointSpace[i].iterNumber - iterMax);

    // 0 <= color_scale <= 255;
    color_scale = (int)(iter * colrUnit);
    int green = std::exp(-1 * std::pow((0.5 - color_scale), 2) * green_sharp);
    int red = (int)color_scale;
    int blue = (1 - color_scale) * 0.5;

    pixCol = 0;

    pixCol += red;
    pixCol <<= 8;

    pixCol += green;
    pixCol <<= 8;

    pixCol += blue;
    pixCol <<= 8;

    const unsigned char alpha = 255;
    pixCol += alpha;
    buf[i] = pixCol;
  };

  {
    std::lock_guard<std::mutex> locLock(MandelBrot<T>::_mandelMtx);
    MandelBrot<T>::_buffer.reset(buf);
    buf = NULL;
  }
  return MandelBrot<T>::_buffer;
};
