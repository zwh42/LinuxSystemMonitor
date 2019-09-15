#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include<iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    //std::cout<<"os = "<<os<<", kernel = "<<kernel<<std::endl;
    //std::cin.get();
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float memory_utilization = 0.0;
  long memory_total = 1, memory_free = 0, memory_used = 0; //memory_total = 1 to avoid divided by 0
  string line, key, value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) { 
        if (key == "MemTotal:"){
          memory_total = std::stol(value);
        }
        if (key == "MemFree:"){
          memory_free = std::stol(value); 
        }
      }
    }  
  }

  memory_used = memory_total - memory_free;
  memory_utilization = 1.0 * memory_used / memory_total;

  return memory_utilization; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, uptime_s;
  long uptime_l = -1;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_s;
    uptime_l = std::stol(uptime_s);
  }

  return uptime_l; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return ActiveJiffies() + IdleJiffies(); 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { 
  return 0; 
  }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  long active_jiffies = 0;
  string cpu, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
   if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq \
      >> softirq >> steal >> guest >> guest_nice;

    // std::cout<<"user = "<<user<<", idle = "<<idle<<std::endl;  
    active_jiffies = user + nice + system + irq + softirq + steal;
  }
  return active_jiffies; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  long idle_jiffies = 0;
  string cpu, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
   if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq \
      >> softirq >> steal >> guest >> guest_nice;
    idle_jiffies =  idle + iowait;
  }
  return idle_jiffies;   
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int total_processes = 0;
  
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) { 
        if (key == "processes"){
          total_processes = std::stoi(value);
          break;
        }
      }
    }  
  }
  
  return total_processes; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int running_processes = 0;  
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) { 
        if (key == "procs_running"){
          running_processes = std::stoi(value);
          break;
        }
      }
    }  
  }
   
   return running_processes; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }