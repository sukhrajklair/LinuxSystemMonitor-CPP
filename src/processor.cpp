#include "processor.h"
#include "linux_parser.h"
//initalize the CPU Usage values
Processor::Processor(void){
 //returns a pair <idle, total>
  auto cpu_util = LinuxParser::CpuUtilization();
  idle = cpu_util.first;
  total = cpu_util.second;
}
//Return the aggregate CPU utilization
float Processor::Utilization() { 
    //returns a pair <idle, total>
    auto cpu_util = LinuxParser::CpuUtilization();
    //new idle = current idle - previous idle
    idle = cpu_util.first - idle;
    //new total = current total - previous total
    total = cpu_util.second - total;
    //utilization %
    return static_cast<float>(total - idle)/total;
 }