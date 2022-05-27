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

long double Process::CpuUtilization() const {
  long double Process_acti =
      (long double)LinuxParser::ActiveJiffies(Process::pid_) /
      sysconf(_SC_CLK_TCK);
  long Processor_start = LinuxParser::UpTime(Process::pid_);
  return (long double)((Process_acti) / Processor_start);
};

string Process::Command() { return LinuxParser::Command(Process::pid_); }

string Process::Ram() { return LinuxParser::Ram(Process::pid_); }

string Process::User() { return LinuxParser::User(Process::pid_); }

long Process::UpTime() { return LinuxParser::UpTime(Process::pid_); }

bool Process::operator<(Process const& a) const {
  return (Process::CpuUtilization() < a.CpuUtilization());
};
