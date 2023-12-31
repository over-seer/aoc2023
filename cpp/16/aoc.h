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
#include <unordered_set>
#include <unordered_map>
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
#include <cmath>
#include <optional>

template<class T>
std::set<T> intersect(const std::set<T> & s1, const std::set<T> & s2) {
    std::set<T> result;
    set_intersection(s1.begin(),s1.end(),s2.begin(),s2.end(),inserter(result,result.begin()));
    return result;
}

inline std::vector<std::string> split(const std::string & line, char c) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string s;
    while(std::getline(ss,s,c)) {
        result.push_back(s);
    }
    return result;
}

template <size_t N>
std::array<std::string,N> splitn(const std::string & line) {
    std::array<std::string,N> result;
    std::stringstream ss(line);
    std::string s;
    size_t i = 0;
    for(size_t i = 0; i<N; i++) {
        ss >> result[i];
    }
    return result;
}


// trim from start (in place)
inline void ltrim_in_place(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
inline void rtrim_in_place(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
inline void trim_in_place(std::string &s) {
    rtrim_in_place(s);
    ltrim_in_place(s);
}

// trim from start (copying)
inline std::string ltrim(std::string s) {
    ltrim_in_place(s);
    return s;
}

// trim from end (copying)
inline std::string rtrim(std::string s) {
    rtrim_in_place(s);
    return s;
}

// trim from both ends (copying)
inline std::string trim(std::string s) {
    trim_in_place(s);
    return s;
}

inline auto read_ip(std::string s){
    std::vector<std::string> lines;
    std::string line;
    std::ifstream ip(s);
    while(std::getline(ip,line))
        lines.push_back(line);
    return lines;
}

inline auto read_paragraph_ip(std::string s){
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

template<class T>
std::set<T> operator+=(std::set<T> & a, const std::set<T> & b) {
    for(auto & k : b) {
        a.insert(k);
    }
    return a;
}

namespace std {
template <typename T, std::size_t N>
struct hash<std::array<T,N>> {
    std::size_t operator()(const std::array<T, N>& arr) const {
        std::hash<T> hasher;
        std::size_t hash_value = 0;
        
        for (const auto& element : arr) {
            hash_value ^= hasher(element) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
        }

        return hash_value;
    }
};
}

#endif
