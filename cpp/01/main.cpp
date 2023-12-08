#include "aoc.h"

using namespace std;

string only_digits(string s) {
    string op;
    for(char c : s) {
        if(c >= '0' && c <= '9') op.push_back(c);
    }
    return op;
}

string only_nums(string s) {
    map<string,string> nums = {
        {"zero","0"},
        {"one","1"},
        {"two","2"},
        {"three","3"},
        {"four","4"},
        {"five","5"},
        {"six","6"},
        {"seven","7"},
        {"eight","8"},
        {"nine","9"}
    };

    for(int i = 0; i < 10; i++) {
        string s = to_string(i);
        nums.insert({s,s});
    }

    string op;
    for(size_t i = 0; i < s.size(); i++) {
        string subs = s.substr(i);
        for(auto [s1,s2] : nums) {
            if(subs.starts_with(s1)) {
                op += s2;
                break;
            }
        }
    }
    return op;
}

void part1(const string & fn) {
    auto ip = read_ip(fn);
    size_t ans = 0;
    for(auto line : ip) {
        string s1 = only_digits(line);
        string s2;
        s2.push_back(s1.front());
        s2.push_back(s1.back());
        size_t n = stoi(s2);
        ans += n;
    }
    cout << fn << " day 1 part 1 answer = " << ans << endl;
}

void part2(const string & fn) {
    auto ip = read_ip(fn);
    size_t ans = 0;
    for(auto line : ip) {
        string s1 = only_nums(line);
        string s2;
        s2.push_back(s1.front());
        s2.push_back(s1.back());
        size_t n = stoi(s2);
        ans += n;
    }
    cout << fn << " day 1 part 2 answer = " << ans << endl;
}

int main() {
    //part1("test_input");
    part1("input");
    //part2("test_input2");
    part2("input");
    return 0;
}