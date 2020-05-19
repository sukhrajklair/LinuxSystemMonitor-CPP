#include <string>
#include <iomanip>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int h, m, s;
    h = seconds/3600;
    m = (seconds - h*3600)/60;
    s = (seconds - h*3600 - m*60);
    std::ostringstream elapsedTime;
    elapsedTime << std::setw(2) << std::setfill('0') << h 
              << ":" << std::setw(2) << std::setfill('0') << m
              << ":" << std::setw(2) << std::setfill('0') << s;
    return elapsedTime.str();
 }