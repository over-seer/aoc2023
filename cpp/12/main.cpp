#include "aoc.h"

using namespace std;

auto parse(const string & fn) {
    vector<pair<string,vector<size_t>>> result;
    auto ip = read_ip(fn);
    for(string line : ip) {
        auto [s1,s2] = splitn<2>(line);
        vector<size_t> nos;
        for(string s : split(s2,','))
            nos.push_back(stoul(s));
        replace(s1.begin(),s1.end(),'.','x');
        result.push_back({s1,nos});
    }

    return result;
}

optional<size_t> fits(const string & s, size_t i0, size_t n) {
    bool ok = true;
    size_t i = i0;
    for(; i < i0 + n; i++) {
        if(s[i] == 'x') {
            ok = false;
            break;
        }
    }
    if(!ok) return nullopt;

    if(s[i] == '#') return nullopt;

    //block of size n fits - now look for a possible gap
    while(i < s.size()) {
        if(s[i] != '#') {
            break;
        }
        i++;
    }

    while(i < s.size()) {
        i++;
        if(s[i] != 'x') {
            break;
        }
    }

    return {i};
}

struct NumCombs {
    string s;
    vector<size_t> nos;
    map<pair<size_t,size_t>,size_t> cache;

    NumCombs(string str, vector<size_t> ns) : s(str), nos(ns) {}
    
    size_t operator()(size_t i0, size_t iblock) {
        if(auto it = cache.find({i0,iblock}); it != cache.end()) return it->second;
        if(iblock == nos.size()) {

            if(find(s.begin() + i0, s.end(), '#') == s.end()) {
                cache[{i0,iblock}] = 1;
                return 1;
            }
            else {
                cache[{i0,iblock}] = 0;
                return 0;
            }
        }
        size_t n = 0;
        size_t minl = iblock == nos.size() ?
            0 : accumulate(nos.begin()+iblock,nos.end(),nos.size()-iblock-1);
        size_t i = i0;
        while(i<s.size()) {
            if(s.size() - i < minl) {
                break;
            }
            char c = s[i];
            if(c != 'x') {
                if(auto opt = fits(s,i,nos[iblock])) {
                    size_t inext = *opt;
                    size_t next_block = iblock + 1;
                
                    //recursive call to self
                    n += operator()(inext,next_block);
                    if(iblock == nos.size()) {
                        //break;
                        cache[{i0,n}] = n;
                        return n;
                    }
                }
            }
            if(c == '#') break;
            i++;
        }

        cache[{i0,iblock}] = n;
        return n;
    }
};

auto unfold(pair<string,vector<size_t>> snos) {
    auto [s,nos] = snos;
    s.push_back('?');
    string s5 = s;
    vector<size_t> nos5 = nos;
    for(size_t i : {0,1,2,3}) {
        s5 += s;
        for(size_t n : nos)
            nos5.push_back(n);
    }
    s5.pop_back();
    return make_pair(s5,nos5);
}

void part1(const string & fn) {
    auto ip = parse(fn);

    size_t ans = 0;
    for(auto [s,nos] : ip) {
        NumCombs num_combs(s,nos);
        size_t n = num_combs(0,0);
        ans += n;
    }
    cout << fn << " day 12 part 1 answer = " << ans << endl;
}


void part2(const string & fn) {
    auto ip = parse(fn);

    size_t ans = 0;
    for(auto snos : ip) {
        snos = unfold(snos);
        auto [s,nos] = snos;
        NumCombs num_combs(s,nos);
        size_t n = num_combs(0,0);
        ans += n;
    }
    cout << fn << " day 12 part 2 answer = " << ans << endl;
}


int main() {
    //part1("test_input");
    part1("input");
    //part2("test_input");
    part2("input");

    return 0;
}
