#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "linux_parser.h"

class Process {
 public:
  Process(int pid);
  std::string User();
  std::string Command();
  int Pid() const;
  float CpuUtilization() const;
  // float GetCpuUtilization();
  std::string Ram();
  long UpTime();
  bool operator>(Process const& a) const;

 private:
  int pid_;
};

#endif