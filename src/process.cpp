#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//Return this process's ID
int Process::Pid() { return pid_; }

//Return this process's CPU utilization
float Process::CpuUtilization() { 
    int utime, stime, cutime, cstime, total_time ;
    
    //processor stats
    std::map<int, long> stats = LinuxParser::ProcessStatParser(pid_);
    utime = stats[14];
    stime = stats[15];
    cutime = stats[16];
    cstime = stats[17];

    //total time in seconds (converted from clock ticks)
    total_time = (utime + stime + cutime + cstime)/sysconf(_SC_CLK_TCK);

    cpu_util = static_cast<float>(total_time)/uptime;
    return cpu_util;
}

//Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

//Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

//Return the age of this process (in seconds)
long int Process::UpTime() { 
    int sys_uptime = LinuxParser::UpTime();
    //ProcessStatParser(pid)[22] returns starttime in clock ticks
    //divide by sysconf(_SC_CLK_TCK) to convert to seconds
    int starttime = LinuxParser::ProcessStatParser(pid_)[22]/sysconf(_SC_CLK_TCK);
    int uptime = sys_uptime - starttime;
    return uptime;
 }

//Overload the "less than" comparison operator for Process objects
//
bool Process::operator<(Process const& a) const { 
    return (cpu_util < a.cpu_util);
 }