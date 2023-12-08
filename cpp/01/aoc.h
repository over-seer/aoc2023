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
