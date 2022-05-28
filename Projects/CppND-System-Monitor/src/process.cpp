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

int Process::Pid() const { return Process::pid_; };

float Process::CpuUtilization() const {
  int jiffies = sysconf(_SC_CLK_TCK);
  // Process_acti in Second
  float Process_acti = (LinuxParser::ActiveJiffies(Process::pid_) / jiffies);
  float Process_start = LinuxParser::UpTime(Process::pid_) / jiffies;
  long Processor_start = LinuxParser::UpTime();
  long time_inter = (Processor_start - Process_start);
  return (float)(Process_acti / (time_inter * 1.0));
};

string Process::Command() { return LinuxParser::Command(Process::pid_); }

string Process::Ram() { return LinuxParser::Ram(Process::pid_); }

string Process::User() { return LinuxParser::User(Process::pid_); }

long Process::UpTime() { return LinuxParser::UpTime(Process::pid_); }

bool Process::operator<(Process const& a) const {
  return (Process::CpuUtilization() < a.CpuUtilization());
};
