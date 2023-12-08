#ifndef MY_AOC_HEADER_H
#define MY_AOC_HEADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>
#include <array>
#include <set>
#include <map>
#include <list>
#include <utility>
#include <tuple>
#include <deque>
#include <numeric>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <regex>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

auto read_ip(std::string s){
    std::vector<std::string> lines;
    std::string line;
    std::ifstream ip(s);
    while(std::getline(ip,line))
        lines.push_back(line);
    return lines;
}

auto read_paragraph_ip(std::string s){
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> para;
    auto lines = read_ip(s);
    std::set<size_t> breaks = {0,lines.size()};
    for(auto line : lines) {
        if(!line.empty()) {
            para.push_back(line);
        } else {
            if(!para.empty()) result.push_back(para);
            para.clear();
        }
    }
    if(!para.empty()) result.push_back(para);
    return result;
}

#endif
