//Note you can solve n(n+t) > d for n using quadratic formula
//(or similar) - here because of rounding errors we use it to get
//a good first guess then iterate a few number below and above

#include "aoc.h"

using namespace std;

auto parse(const string & fn) {
    auto ip = read_ip(fn);
    array<vector<int64_t>,2> dt;
    vector<array<int64_t,2>> result;
    for(size_t i : {0,1}) {
        string line = ip[i];
        stringstream ss(line);
        string word;
        int64_t f;
        ss >> word; // first column ignore
        while(ss >> f) {
            dt[i].push_back(f);
        }
    }
    return dt;
}

auto parse2(const string & fn) {
    auto ip = read_ip(fn);
    array<int64_t,2> td;
    vector<array<int64_t,2>> result;
    for(size_t i : {0,1}) {
        string line = ip[i];
        stringstream ss(line);
        string word;
        int64_t f;
        ss >> word; // first column ignore
        string sn;
        while(ss >> word) {
            sn += word;
        }
        td[i] = stol(sn);
    }
    return td;
}

int64_t disteq(int64_t t, int64_t n) {
    return n*(t - n);
}

auto g(double t, double d) {
    return pair<double,double>({-(-t+sqrt(t*t-4*d))/2.},{-(-t-sqrt(t*t-4*d))/2.});
}

int64_t f(int64_t time, int64_t dist) {
    double dtime = static_cast<double>(time);
    double ddist = static_cast<double>(dist);
    auto [t1,t2] = g(dtime,ddist);
    //cout << t1 << "  " << t2 << endl;
    
    int64_t i1 = (int64_t)t1;
    int64_t i2 = (int64_t)t2;
    int64_t n1,n2;
    for(int64_t i = i1 - 2; i < i1 + 4; i++) {
        if(disteq(time,i) > dist) {
            n1 = i;
            break;
        }
    }

    for(int64_t i = i2 - 2; i < i2 + 4; i++) {
        if(disteq(time,i) <= dist) {
            n2 = i;
            break;
        }
    }

    return n2 - n1;
}

void part1(const string & fn) {
    auto td = parse(fn);
    int64_t ans = 1;
    for(size_t i = 0; i < td[0].size(); i++) {
        auto t = td[0][i];
        auto d = td[1][i];
        ans *= f(t,d);
    }
    cout << fn << " part 1 ans = " << ans << endl;
}

void part2(const string & fn) {
    auto [t,d] = parse2(fn);
    cout << fn << " part 2 ans = " << f(t,d) << endl;
}

int main() {
    //part1("test_input");
    part1("input");
    //part2("test_input");
    part2("input");
    return 0;
}