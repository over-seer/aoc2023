#include "aoc.h"

using namespace std;
array<size_t,3> parse_cubes(const string & cubes) {
    array<size_t,3> rgb = {0,0,0};
    stringstream ss(cubes);
    string s;
    while(getline(ss,s,',')) {
        stringstream ss2(s);
        string sn, srgb;
        ss2 >> sn;
        ss2 >> srgb;
        size_t n = stoi(sn);
        if(srgb == "red") rgb[0] = n;
        else if(srgb == "green") rgb[1] = n;
        else if(srgb == "blue") rgb[2] = n;
        else throw runtime_error("bad colour");
    }
    return rgb;
}

auto parse(const string & fn) {
    auto ip = read_ip(fn);
    vector<vector<array<size_t,3>>> res;
    for(string line : ip) {
        vector<array<size_t,3>> game;
        size_t n;
        stringstream ss(line);
        string s1,s2;
        getline(ss,s1,':');
        getline(ss,s2);
        n = stoul(s1.substr(5));
        stringstream ss2(s2);
        while(getline(ss2,s1,';')) {
            game.push_back(parse_cubes(s1));
        }
        res.push_back(game);
    }
    return res;
}

bool possible(const array<size_t,3> & elf_cubes, const vector<array<size_t,3>> & game) {
    for(auto & cubes : game)
        for(size_t i : {0,1,2})
            if(cubes[i] > elf_cubes[i]) return false;
    return true;
}

auto minimum_set(const vector<array<size_t,3>> & game) {
    array<size_t,3> rgb = {0,0,0};
    for(auto & cubes : game)
        for(size_t i : {0,1,2})
            rgb[i] = max(rgb[i],cubes[i]);
    return rgb;
}

void part1(const string & fn) {
    const auto ip = parse(fn);
    size_t ans = 0;
    for(size_t i = 0; i < ip.size(); i++) {
        if(possible({12,13,14},ip[i])) {
            ans += i + 1;
        }
    }
    cout << fn << " day 2 part 1 answer = " << ans << endl;
}

void part2(const string & fn) {
    const auto ip = parse(fn);
    size_t ans = 0;
    for(auto & game : ip) {
        auto rgb = minimum_set(game);
        ans += rgb[0] * rgb[1] * rgb[2];
    }
    cout << fn << " day 2 part 2 answer = " << ans << endl;
}

int main() {
    //part1("test_input");
    part1("input");
    //part2("test_input");
    part2("input");
    return 0;
}