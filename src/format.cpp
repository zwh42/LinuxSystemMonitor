#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int h, m, s;
    string result = "";
    h = seconds / 3600;
    m = (seconds - h * 3600) / 60; 
    s = seconds % 60;
    if (h < 10){
        result += "0";
    }
    result += std::to_string(h) + ":" ;
    if (m < 10){
        result += "0";
    }
    result += std::to_string(m) + ":";
    if (s < 10){
        result += "0";
    }
    result += std::to_string(s);
      

    return result;
}