#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

const int user_hz = sysconf(_SC_CLK_TCK);

template <typename T1, typename T2>
int find_elem_index(const T2* vec, const T1& elem) {
  int ind = 0;
  for (int it = 0; it < (int)vec->size(); it++) {
    ind++;

    if ((*vec)[it] == elem) break;
  }
  return ind;
};

// System
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream ifs(kOSPath);
  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream ls(line);
      while (ls >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
};

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream ifs(kProcDirectory + kVersionFilename);
  if (ifs.is_open()) {
    std::getline(ifs, line);
    std::istringstream ls(line);
    ls >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string key;
  string value;
  string unit;
  string line;
  float tot_mem = 0;   // = 0;
  float free_mem = 0;  // = 0;
  int stop_iter = 2;

  std::ifstream fs(kProcDirectory + kMeminfoFilename);
  if (fs.is_open()) {
    while (std::getline(fs, line)) {
      std::istringstream ls(line);
      ls >> key >> value >> unit;
      if (stop_iter == 0) {
        break;
      };

      if (key == "MemTotal:") {
        tot_mem = std::stof(value);
        stop_iter--;
      };

      if (key == "MemFree:") {
        free_mem = std::stof(value);
        stop_iter--;
      };
    }
  }

  return (tot_mem - free_mem) / tot_mem;
};

long LinuxParser::UpTime() {
  string uptime, idltime;
  string line;
  std::ifstream fs(kProcDirectory + kUptimeFilename);

  if (fs.is_open()) {
    if (std::getline(fs, line)) {
      std::istringstream ls(line);
      ls >> uptime >> idltime;
    }
  };
  // uptime values in seconds
  return (std::stol(uptime));
}

int LinuxParser::TotalProcesses() {
  string TotProc;
  string search_TotProc;
  string line;
  std::ifstream fs(kProcDirectory + kStatFilename);

  if (fs.is_open()) {
    while (std::getline(fs, line)) {
      std::istringstream ls(line);
      if (ls >> search_TotProc) {
        if (search_TotProc == "processes") {
          ls >> TotProc;
        } else {
          continue;
        }
      }
    }
  }

  return std::stoi(TotProc);
}

int LinuxParser::RunningProcesses() {
  string RunProc;
  string search_RunProc;
  string line;
  std::ifstream fs(kProcDirectory + kStatFilename);

  if (fs.is_open()) {
    while (std::getline(fs, line)) {
      std::istringstream ls(line);
      if (ls >> search_RunProc) {
        if (search_RunProc == "procs_running")
          ls >> RunProc;
        else
          continue;
      }
    }
  }

  return std::stoi(RunProc);
}

// CPUs
long LinuxParser::Jiffies() {
  vector<vector<long>>* cpu_utilz = new vector<vector<long>>{};
  vector<string>* CPUS_str = new vector<string>{};
  LinuxParser::CpuUtilization(cpu_utilz, CPUS_str);

  string cpu = "cpu";
  int y_i = find_elem_index(CPUS_str, cpu);
  vector<long> ActVecc = (*cpu_utilz)[y_i];

  long Jif = 0;

  for (size_t i = LinuxParser::kUser_; i <= LinuxParser::kSteal_; i++) {
    Jif += ActVecc[i];
  };

  delete cpu_utilz;
  delete CPUS_str;
  return Jif;
}

long LinuxParser::ActiveJiffies(const string cpu_n) {
  vector<vector<long>>* cpu_utilz = new vector<vector<long>>{};
  vector<string>* CPUS = new vector<string>{};
  LinuxParser::CpuUtilization(cpu_utilz, CPUS);

  int y_i = find_elem_index(CPUS, cpu_n);
  vector<long> ActVecc = (*cpu_utilz)[y_i];

  long ActJif = 0;

  for (size_t i = 0; i < ActVecc.size(); i++) {
    if (LinuxParser::kIdle_ == i) continue;
    if (LinuxParser::kIOwait_ == i) continue;
    if (LinuxParser::kGuest_ == i) continue;
    if (LinuxParser::kGuestNice_ == i) continue;
    ;
    ActJif += ActVecc[i];
  };

  delete cpu_utilz;
  delete CPUS;
  return ActJif;
}

long LinuxParser::IdleJiffies(string cpu_n) {
  vector<vector<long>>* cpu_utilz = new vector<vector<long>>{};
  vector<string>* CPUS = new vector<string>{};
  LinuxParser::CpuUtilization(cpu_utilz, CPUS);

  int y_i = find_elem_index(CPUS, cpu_n);
  vector<long> ActJifVec = (*cpu_utilz)[y_i];
  long idle_jiffi = ActJifVec[kIdle_] + ActJifVec[kIOwait_];
  delete cpu_utilz;
  delete CPUS;
  return idle_jiffi;
}

void LinuxParser::CpuUtilization(vector<vector<long>>* cpu_utilz,
                                 vector<string>* CPUS) {
  string cpu{""}, line;
  const char* cpu_char;
  string value;

  std::ifstream ifs(kProcDirectory + "/" + kStatFilename);

  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::istringstream ils(line);
      ils >> cpu;
      cpu_char = cpu.c_str();

      if (cpu_char[0] == 'c' && cpu_char[1] == 'p' && cpu_char[2] == 'u') {
        CPUS->push_back(cpu);
        vector<long> add_column = {};
        while (ils >> value) {
          add_column.push_back(std::stol(value));
        }
        cpu_utilz->push_back(add_column);

      } else {
        break;
      }
    };
  };
};

long LinuxParser::ActiveJiffies(int pid) {
  // utime(14), stime(15), cutime(16), cstime(17) in jiffies
  string utime, stime, cutime, cstime;
  string line;

  std::ifstream ifs(kProcDirectory + "/" + std::to_string(pid) + "/" +
                    kStatFilename);
  if (ifs.is_open()) {
    if (std::getline(ifs, line)) {
      std::istringstream ils(line);

      int count = 0;
      while (count < 14) {
        ils >> utime;
        count++;
      };
      ils >> stime >> cutime >> cstime;
    };
  };

  return (std::stol(utime) + std::stol(stime) + std::stol(cutime) +
          std::stol(cstime));
};

string LinuxParser::Command(int pid) {
  string line;
  string Command;
  std::ifstream ifs(kProcDirectory + "/" + std::to_string(pid) + "/" +
                    kCmdlineFilename);
  if (ifs.is_open()) {
    std::getline(ifs, line);
    std::istringstream ils(line);
    ils >> Command;
  };
  return Command;
}

string LinuxParser::Ram(int pid) {
  string srch_key = "VmRSS:";
  string key, value;
  string line;
  string nul;

  std::ifstream ifs(kProcDirectory + "/" + std::to_string(pid) + "/" +
                    kStatusFilename);

  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::istringstream ils(line);
      ils >> key >> value >> nul;
      if (key == srch_key) {
        break;
      };
    };
  }
  // KB to MB
  long val = std::stol(value) / 1000;

  return to_string(val);
}

string LinuxParser::Uid(int pid) {
  string srch_key = "Uid:";
  string key, value;
  string line;
  string nul;

  std::ifstream ifs(kProcDirectory + "/" + std::to_string(pid) + "/" +
                    kStatusFilename);

  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::istringstream ils(line);
      ils >> key;
      if (key == srch_key) {
        ils >> value;
        break;
      };
    };
  }
  return value;
};

string LinuxParser::User(int pid) {
  string uid, user;
  string uid_srch;
  string line;
  string nul;

  uid_srch = Uid(pid);

  std::ifstream ifs(kPasswordPath);

  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');

      std::istringstream ils(line);
      ils >> user >> nul >> uid;

      if (uid_srch == uid) break;
    };
  };
  return user;
};

long LinuxParser::UpTime(int pid) {
  string line;
  string uptime;
  std::ifstream ifs(kProcDirectory + "/" + std::to_string(pid) + "/" +
                    kStatFilename);

  if (ifs.is_open()) {
    if (std::getline(ifs, line)) {
      std::istringstream ils(line);

      int count = 0;
      while (count < 22) {
        ils >> uptime;
        count++;
      };
    };
  };
  // to second
  return std::stol(uptime);
};
