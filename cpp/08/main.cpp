#include "aoc.h"

using namespace std;

auto parse(const string & fn) {
    auto ip = read_paragraph_ip(fn);
    auto dirs = ip.at(0).at(0);
    map<string,array<string,2>> nodes;
    for(auto line : ip.at(1)) {
        nodes[line.substr(0,3)] = {line.substr(7,3),line.substr(12,3)};
    }
    return make_pair(dirs,nodes);
}

struct Dirs {
    size_t ix = 0;
    vector<size_t> lr;
    Dirs(const string & s) : ix(s.size()) {
        lr.resize(s.size());
        for(size_t i = 0; i < s.size(); i++) {
            lr.at(i) = s.at(i) == 'L' ? 0 : 1;
        }
    }
    size_t next() {
        if(ix == lr.size()) ix = 0;
        return lr[ix++];
    }
};

void part1(const string & fn) {
    auto [d,nodes] = parse(fn);
    Dirs dirs(d);
    int ans = 0;
    string el = "AAA";
    while(el != "ZZZ") {
        size_t ix = dirs.next();
        el = nodes.at(el)[ix];
        ans++;
    }
    cout << fn << " day 8 part 1 answer = " << ans << endl; 
}

void investigate(const string & fn) {
    auto [d,nodes] = parse(fn);
    Dirs dirs(d);
    size_t i = 0;
    map<pair<string,size_t>,size_t> visits;
    string el = "AAA";
    while(true) {
        size_t ix = dirs.next();
        el = nodes.at(el)[ix];
        if(visits.contains({el,dirs.ix})) {
            cout << i << " : " << el << dirs.ix << " " << visits[{el,dirs.ix}] << " " <<  i - visits[{el,dirs.ix}]<<endl;
        }
        visits[{el,dirs.ix}] = i;
        i++;
    }
}

size_t cycle_size(string d, const map<string,array<string,2>> & nodes, string s) {
    Dirs dirs(d);
    size_t i = 0;
    map<pair<string,size_t>,size_t> visits;
    while(true) {
        size_t ix = dirs.next();
        s = nodes.at(s)[ix];
        if(visits.contains({s,dirs.ix})) {
            return i - visits[{s,dirs.ix}];
        }
        visits[{s,dirs.ix}] = i;
        i++;
    }
    return 0;
}

auto zed_ns(string d, const map<string,array<string,2>> & nodes, string s) {
    Dirs dirs(d);
    size_t i = 0;
    map<string,size_t> visits;
    while(true) {
        size_t ix = dirs.next();
        s = nodes.at(s)[ix];
        if(s[2] == 'Z') {
            if(visits.contains({s})) {
                return visits[s];
            }
            visits.insert({s,i});
        }
        i++;
    }
}

size_t gcd(size_t m, size_t n) {
    while(m!=0) {
        if(m<n) {
            swap(m,n);
        }
        m%=n;
    }
    return n;
}

size_t lcm(size_t a, size_t b) {
    return a * b / gcd(a,b);
}

template<class It>
auto lcm(It first, It last) {
    auto result = *first;
    ++first;
    for ( ;first != last; ++first)
      result = lcm(result,*first);
   return(result);
}

void part2(const string & fn) {
    const auto [d,nodes] = parse(fn);
    vector<size_t> periods;
    for(auto & [a,bc] : nodes) {
        if(a[2] == 'A') {
            size_t period = cycle_size(d,nodes,a);
            size_t offset = zed_ns(d,nodes,a);
            // NOTE that print below confirms that for the input
            // this is a least common multiple problem
            //cout << a << ":" << offset << "," << period << endl;
            periods.push_back(period);
        }
    }

    size_t ans = lcm(periods.begin(),periods.end());
    
    cout << fn << " day 8 part 2 answer = " << ans << endl; 
    //Dirs dirs(d);
}


int main() {
    //part1("test_input");
    part1("input");
    //part2("test_input2");
    part2("input");
    //investigate("input");
    return 0;
}
