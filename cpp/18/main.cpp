#include "aoc.h"

using namespace std;

enum class Dir {U,D,L,R};
using enum Dir;

array<int64_t,2> dir(Dir d) {
    switch(d) {
        case U:
            return {-1,0};
        case D:
            return {1,0};
        case L:
            return {0,-1};
        case R:
            return {0,1};
        default:
            throw runtime_error("bad Dir");
    }
}

Dir get_dir(char c) {
    switch(c) {
        case 'U': return U;
        case 'D': return D;
        case 'L': return L;
        case 'R': return R;
        default : throw runtime_error("bad Dir");
    }
}

auto parse1(const string & fn) {
    vector<pair<Dir,int64_t>> result;
    auto ip = read_ip(fn);
    for(string line : ip) {
        auto [sd,si,_] = splitn<3>(line);
        result.push_back({get_dir(sd.front()),stoi(si)});
    }
    return result;
}

Dir ctodir(char c) {
    //0 means R, 1 means D, 2 means L, and 3 means U.
    switch (c) {
        case '0': return R;
        case '1': return D;
        case '2': return L;
        case '3': return U;
        default : throw runtime_error("bad dir char");
    }
}

auto parse2(const string & fn) {
    vector<pair<Dir,int64_t>> result;
    auto ip = read_ip(fn);
    for(string line : ip) {
        auto [_d,_i,si] = splitn<3>(line);
        char c = si.at(7);
        si = si.substr(2,5);
        uint64_t x;   
        stringstream ss;
        ss << hex << si;
        ss >> x;
        int64_t i = (int64_t)x;
        //cout << si << " " << i << endl;
        result.push_back({ctodir(c),i});
    }
    return result;
}

auto outer_perimeter(vector<pair<Dir,int64_t>> ip) {
    vector<array<int64_t,2>> pts;
    int64_t len = 0;
    int64_t r = 0;
    int64_t c = 0;
    ip.push_back(ip.front());
    for(size_t i = 0; i < ip.size() - 1; i++) {
        auto [d1,n] = ip.at(i);
        auto [d2,_] = ip.at(i+1);
        auto [dr,dc] = dir(d1);
        len += n;
        r += n*dr;
        c += n*dc;
        int64_t lr, lc;
        //outside for clockwise pts
        if(d1 == U && d2 == R)      {lr = r;     lc = c;}
        else if(d1 == R && d2 == D) {lr = r;     lc = c + 1;}
        else if(d1 == D && d2 == L) {lr = r + 1; lc = c + 1;}
        else if(d1 == L && d2 == U) {lr = r + 1; lc = c;}
        else if(d1 == U && d2 == L) {lr = r + 1; lc = c;}
        else if(d1 == L && d2 == D) {lr = r + 1; lc = c + 1;}
        else if(d1 == D && d2 == R) {lr = r;     lc = c + 1;}
        else if(d1 == R && d2 == U) {lr = r;     lc = c;}
        else throw runtime_error("bad corner");
        pts.push_back({lr,lc});
    }

    return make_pair(pts,len);
}

int64_t area(vector<array<int64_t,2>> pts) {
    pts.push_back(pts.front());
    int64_t a = 0;
    for(size_t i = 0; i < pts.size() - 1; i++) {
        auto [x1,y1] = pts.at(i);
        auto [x2,y2] = pts.at(i + 1);
        a += (x2*y1 - x1*y2);
    }
    return a/2;
}

void part1(const string & fn) {
    auto ip = parse1(fn);
    auto [pts,plen] = outer_perimeter(ip);
    //for(auto [x,y] : pts) cout << x << " , " << y << endl;
    int64_t ans1 = area(pts);
    //cout << ans1 << endl;
    if(ans1 < 0) ans1 = -ans1 + plen;
    cout << fn << " day 18 part 1 answer = " << ans1 << endl;
}

void part2(const string & fn) {
    auto ip = parse2(fn);
    auto [pts,plen] = outer_perimeter(ip);
    int64_t ans2 = area(pts);
    //cout << ans2 << endl;
    if(ans2 < 0) ans2 = -ans2 + plen;
    cout << fn << " day 18 part 2 answer = " << ans2 << endl;
}

int main() {
    //part1("test_input");
    //part2("test_input");
    part1("input");
    part2("input");
    return 0;
}
