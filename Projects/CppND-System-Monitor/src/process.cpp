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

// TODO: Return this process's ID
int Process::Pid() { return Process::pid_; };

// TODO: Return this process's CPU utilization
double Process::CpuUtilization() const {
  double Process_acti =
      (double)LinuxParser::ActiveJiffies(Process::pid_) / sysconf(_SC_CLK_TCK);
  double Processor_start = LinuxParser::UpTime(Process::pid_);
  return (Process_acti) / Processor_start;
};

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Process::pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Process::pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Process::pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Process::pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  if (Process::CpuUtilization() < a.CpuUtilization())
    return true;
  else
    return false;
}