#include <complex>
#include <mutex>
#include <thread>
#include <vector>

#include "mandelbrot.h"

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
};

template <class T>
int MandelBrot<T>::searchMandelBrotSetPoint(std::complex<T> &c) {
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
void MandelBrot<T>::constructMandelBrot() {
  for (MandelData<T, int> &mandelData : MandelBrot<T>::_pointSpace) {
    int iterNumberWithComlxC = searchMandelBrotSetPoint(mandelData.comPoint);
    mandelData.iterNumber = iterNumberWithComlxC;
  }
};
template <class T>
void MandelBrot<T>::constructMandelBrotConcurrent() {
  std::vector<std::future<int>> ftrs;

  for (MandelData<T, int> &mandelData : MandelBrot<T>::_pointSpace) {
    ftrs.emplace_back

        int iterNumberWithComlxC searchMandelBrotSetPoint(mandelData.comPoint);
    mandelData.iterNumber = iterNumberWithComlxC;
  }
};

template <class T>
void MandelBrot<T>::checkDataPoints() {
  for (auto &mandelData : MandelBrot<T>::_pointSpace) {
    std::cout << "DC: real " << mandelData.comPoint.real() << " DC: img "
              << mandelData.comPoint.imag()
              << " iter : " << mandelData.iterNumber << "\n";
  };
}