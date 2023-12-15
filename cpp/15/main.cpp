#include "aoc.h"

using namespace std;

vector<string> parse1(const string & fn) {
    auto lines = read_ip(fn);
    string line = lines.front();
    vector<string> result = split(line,',');
    return result;
}

int hash_algorithm(const string & label) {
    int i = 0;
    for(char c : label) {
        i += (int)c;
        i *= 17;
        i %= 256;
    }
    return i;
}

void part1(const string & fn) {
    auto ip = parse1(fn);
    int ans1 = 0;
    for(string s : ip) {
        int i = hash_algorithm(s);
        ans1 += i;
    }
    cout << fn << " day 15 part 1 answer = " << ans1 << endl;
}

auto parse2(const vector<string> & ip) {
    vector<pair<string,int>> result;
    for(string s : ip) {
        if(auto i = s.find('='); i != string::npos) {
            result.push_back({s.substr(0,i),stoi(s.substr(i+1))});
        } else {
            result.push_back({s.substr(0,s.size()-1),-1});
        }
    }
    return result;
}

int power(int box, const list<pair<string,int>> & lenses) {
    int fp = 0;
    int slot = 0;
    for(auto & [_,len] : lenses) {
        slot++;
        fp += (box + 1) * slot * len;
    }
    return fp;
}

void part2(const string & fn) {
    const auto ip = parse2(parse1(fn));

    unordered_map<int,list<pair<string,int>>> hashmap;

    for(auto & slot : ip) {
        auto & [label,len] = slot;
        if(len == -1) {
            //not that line below will create an emplty list key-value pair in the map if it
            //doesn't already exist but it doesn't matter
            hashmap[hash_algorithm(label)].remove_if([&](auto & p){return p.first == slot.first;});
            //note the line below works with g++ but not clang++ - clang bug?
            //hashmap[hash_algorithm(label)].remove_if([&](auto & p){return p.first == label;});
        } else {
            auto & lenses = hashmap[hash_algorithm(label)];
            auto eq = [&](const pair<string,int> & p){return p.first == slot.first;};
            //auto eq = [&](const pair<string,int> & p){return p.first == label;};
            auto it = find_if(lenses.begin(),lenses.end(),eq);
            if(it == lenses.end()) {
                lenses.push_back({label,len});
            } else {
                it->second = len;
            }
        }
    }

    int ans2 = 0;
    for(const auto & [i,lenses] : hashmap) {
        ans2 += power(i,lenses);
    }
    cout << fn << " day 15 part 2 answer = " << ans2 << endl;
}

int main() {
    //part1("test_input");
    part1("input");
    //part2("test_input");
    part2("input");
    return 0;
}
