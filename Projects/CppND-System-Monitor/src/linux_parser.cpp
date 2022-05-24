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

//###############################
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
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
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
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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
  long int value;
  string unit;
  long int tot_mem = 0;
  long int free_mem = 0;
  int stop_iter = 2;

  std::ifstream fs(kProcDirectory + kMeminfoFilename);
  if (fs.is_open()) {
    while (fs >> key >> value >> unit) {
      if (stop_iter == 0) {
        break;
      };

      if (key == "MemTotal") {
        tot_mem = value;
        stop_iter--;
      };

      if (key == "MemFree") {
        free_mem = value;
        stop_iter--;
      };
    }
  }

  return (free_mem / (float)tot_mem);
};

long LinuxParser::UpTime() {
  long double uptime = 0;
  long double idltime = 0;
  std::ifstream fs(kProcDirectory + kUptimeFilename);
  if (fs.is_open()) {
    fs >> uptime >> idltime;
  };
  return uptime;
}

int LinuxParser::TotalProcesses() {
  int TotProc;
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

  return TotProc;
}

int LinuxParser::RunningProcesses() {
  int RunProc;
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

  return RunProc;
}

// CPUs
long LinuxParser::Jiffies() {
  int tick_p_sc = sysconf(_SC_CLK_TCK);
  return (long int)(UpTime() * tick_p_sc);
}

//#######################################
long LinuxParser::ActiveJiffies(const string cpu_n) {
  vector<vector<long>>* cpu_utilz = new vector<vector<long>>{};
  vector<string>* CPUS = new vector<string>{};
  LinuxParser::CpuUtilization(cpu_utilz, CPUS);

  int y_i = find_elem_index(CPUS, cpu_n);
  vector<long> ActVecc = (*cpu_utilz)[y_i];
  long ActJif = 0;
  for (const long& i : ActVecc) {
    if (LinuxParser::kIdle_ == 3) continue;
    if (LinuxParser::kIOwait_ == 4) continue;
    ActJif += i;
  }
  return ActJif;
}

//###################################################
long LinuxParser::IdleJiffies(string cpu_n) {
  vector<vector<long>>* cpu_utilz = new vector<vector<long>>{};
  vector<string>* CPUS = new vector<string>{};
  LinuxParser::CpuUtilization(cpu_utilz, CPUS);

  int y_i = find_elem_index(CPUS, cpu_n);
  vector<long> ActJifVec = (*cpu_utilz)[y_i];
  return ActJifVec[kIdle_];
}

//#############################
void LinuxParser::CpuUtilization(vector<vector<long>>* cpu_utilz,
                                 vector<string>* CPUS) {
  string cpu{""}, line;
  const char* cpu_char;
  string value;

  std::ifstream ifs(kProcDirectory + kStatFilename);

  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::istringstream ils(line);
      ils >> cpu;
      cpu_char = cpu.c_str();

      if (cpu_char[0] == 'c' && cpu_char[1] == 'p' && cpu_char[2] == 'u') {
        CPUS->push_back(cpu);
        vector<long> add_column;
        while (ils >> value) {
          long value_ = std::stol(value);
          add_column.push_back(value_);
        }
        cpu_utilz->push_back(add_column);

      } else {
        break;
      }
    };
  };
};

long LinuxParser::ActiveJiffies(int pid) {
  string utime, stime, cutime, cstime, line;

  std::ifstream ifs(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
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

  return (std::stoi(utime) + std::stoi(stime) + std::stoi(cutime) +
          std::stoi(cstime));
};

string LinuxParser::Command(int pid) {
  string line;

  std::ifstream ifs(kProcDirectory + "/" + std::to_string(pid) +
                    kCmdlineFilename);
  if (ifs.is_open()) {
    std::getline(ifs, line);
  };
  return line;
}

string LinuxParser::Ram(int pid) {
  string srch_key = "VmSize:";
  string key, value;
  string line;
  string nul;

  std::ifstream ifs(kProcDirectory + "/" + std::to_string(pid) +
                    kStatusFilename);

  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::istringstream ils(line);
      ils >> key;
      if (key == srch_key) {
        ils >> value;
        ils >> nul;
        break;
      };
    };
  }
  return value;
}

string LinuxParser::Uid(int pid) {
  string srch_key = "Uid:";
  string key, value;
  string line;
  string nul;

  std::ifstream ifs(kProcDirectory + "/" + std::to_string(pid) +
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
}

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
  string uptime, line;

  std::ifstream ifs(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);

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

  return std::stoi(uptime);
};