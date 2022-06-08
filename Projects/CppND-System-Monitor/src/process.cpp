#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  this->pid_ = pid;
  // this->CPUUtils = this->CpuUtilization();
};

int Process::Pid() const { return Process::pid_; };

float Process::CpuUtilization() const {
  int user_hz = sysconf(_SC_CLK_TCK);
  // Process_acti in user_hz
  long PIDActi = LinuxParser::ActiveJiffies(Process::pid_);
  long Sysup = LinuxParser::UpTime() * user_hz;
  long ProcStart = LinuxParser::UpTime(Process::pid_) * user_hz;
  float cpu_util = (float)(PIDActi / (Sysup - ProcStart));
  return cpu_util;
};

// float Process::GetCpuUtilization() { return this->CPUUtils; }

string Process::Command() { return LinuxParser::Command(Process::pid_); }

string Process::Ram() { return LinuxParser::Ram(Process::pid_); }

string Process::User() { return LinuxParser::User(Process::pid_); }

long Process::UpTime() { return LinuxParser::UpTime(Process::pid_); }

bool Process::operator>(Process const& a) const {
  return (Process::CpuUtilization() > a.CpuUtilization());
};
