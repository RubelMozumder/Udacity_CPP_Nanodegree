#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

bool SortDecision(Process const& a, Process const& b) {
  if (a > b)
    return true;
  else
    return false;
};

Processor& System::Cpu() {
  // System::cpu_ = Processor();

  return System::cpu_;
}

vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  this->processes_.clear();
  for (auto i : pids) {
    Process process = Process(i);
    this->processes_.push_back(process);
  };
  std::sort(this->processes_.begin(), this->processes_.end(), SortDecision);
  return this->processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); };

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long System::UpTime() { return LinuxParser::UpTime(); }
