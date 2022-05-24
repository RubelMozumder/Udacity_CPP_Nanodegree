#include "format.h"

#include <string>

using std::string;
using std::to_string;

string ElapsedTime(int seconds) {
  const int h_in_s = 3600;
  const int m_in_s = 60;
  int h = seconds / h_in_s;
  int m = (seconds % h_in_s) / m_in_s;
  int s = (seconds % h_in_s) % m_in_s;

  return (to_string(h) + ":" + to_string(m) + ":" + to_string(s));
};
