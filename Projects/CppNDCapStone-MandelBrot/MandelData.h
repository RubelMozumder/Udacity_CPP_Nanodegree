#ifndef MANDELDATA_H
#define MANDELDATA_H

#include <complex>

struct MandelData {
  std::complex<double> comPoint;
  int iterNumber = 0;
};

#endif