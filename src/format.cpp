#include <string>

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
    return std::to_string(h)+ ":" + std::to_string(m) + ":" + std::to_string(s);
 }