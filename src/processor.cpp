#include "processor.h"
#include "linux_parser.h"

//Return the aggregate CPU utilization
float Processor::Utilization() { 
    float util;
    //returns a pair <idle, total>
    auto cpu_util = LinuxParser::CpuUtilization();
    //new idle = current idle - previous idle
    idle = cpu_util.first - idle;
    //new total = current total - previous total
    total = cpu_util.second - total;
    //utilization %
    util = (total - idle)/static_cast<float>(total);
    return util;
 }