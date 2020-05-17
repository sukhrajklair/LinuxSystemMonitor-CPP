#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read Operating System Name
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

//Reading kernel name
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

//Update this to use std::filesystem
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

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  int memTotal, memFree, memUtilized;
  string line, attribute;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open())
  {
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> attribute >> memTotal;
    std::getline(stream,line);
    linestream.str(line);
    linestream >> attribute >> memFree;

    memUtilized = memTotal - memFree;
  }
  return static_cast<float>(memUtilized)/memTotal;
 }

// Read and return the system uptime
long int LinuxParser::UpTime() { 
  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    lineStream >> uptime;
  }
  return uptime;
 }

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

//Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

//Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
std::pair<int, int> LinuxParser::CpuUtilization() { 
  int user, nice, system, idle, iowait, irq, softirq, steal;
  int total, idle_all, non_idle;
  string line, token;
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::istringstream lineStream(line);
  while (stream.is_open() && token != "cpu")
  {
    //first line contains the CPU utilization data
    std::getline(stream, line);
    lineStream.str(line);
    //stream the line to extract the parameters
    lineStream >> token;
  }
    lineStream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    idle_all = idle+iowait;
    non_idle = user + nice + system + irq + softirq + steal; 
    total = idle_all + non_idle;
    return {idle_all, total};
 }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int processes;
  string line, token;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (stream.is_open() && token != "processes")
  {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    lineStream >> token >> processes;
  }
  
  return processes;
 }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int processes;
  string line, token;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (stream.is_open() && token != "procs_running")
  {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    lineStream >> token >> processes;
  }
  
  return processes;
 }

//Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string cmd;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open())
  {
    std::getline(stream, cmd);
  }
  return cmd;
 }

//Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, token;
  int ram;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  std::istringstream linestream(" ");
  while(stream.is_open() && token != "VmSize:")
  {
    std::getline(stream,line);
    linestream.str(line);
    linestream >> token;
  }
  linestream >> ram;
  ram = ram/1024; //convert from KB to MB
  return to_string(ram);
 }

//Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, token, uid;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  std::istringstream linestream(line);
  while(stream.is_open() && token != "Uid:")
  {
    std::getline(stream,line);
    linestream.str(line);
    linestream >> token;
  }
  linestream >> uid;
  return uid;
}

//Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, user_id, user_name, x;
  std::ifstream stream(kPasswordPath);
  std::istringstream linestream(line);
  string uid = Uid(pid);

  while (stream.is_open() && user_id != uid)
  {
    getline(stream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    linestream.str(line);
    linestream >> user_name >> x >> user_id;
  }
  return user_name;
 }

//Read and return the uptime of a process
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

// Read and return process CPU utilization
std::map<int,long> LinuxParser::ProcessStatParser(int pid) { 
  string line;
  long token;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  std::map<int, long> tokens;
  if(stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    int c = 1;
    while(lineStream >> token)
    {
      tokens[c] = token;
      c++;
    }
  }
  return tokens;
}