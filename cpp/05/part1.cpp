#include "aoc.h"
#include "n_array_impl.h"

using namespace std;
using namespace n_array;

auto parse(const string & fn) {
    const auto ip = read_paragraph_ip(fn);
    //vector<int64_t> result;
    //map<> result;
    pair<
        vector<int64_t>,
        array<vector<array<int64_t,3>>,7>
        > result;
    for(auto & line : ip.at(0)) {
        auto words = split(line,' ');
        for(int64_t i = 1; i < words.size(); i++) {
            //cout << words.at(i) << endl;
            result.first.push_back(stoul(words.at(i)));
        }
    }
    for(int64_t i = 1; i < 8; i++) {
        const vector<string> & para = ip.at(i);
        for(int64_t j = 1; j < para.size(); j++) {
            const string & line = para.at(j);
            //cout << line << endl;
            auto words = split(line,' ');
            array<int64_t,3> nos;
            for(int64_t k = 0; k < 3; k++) {
                nos.at(k) = stoul(words.at(k));
            }
            result.second.at(i-1).push_back(nos);
        }
    }
    return result;
}

int64_t get_loc(int64_t seed, const array<vector<array<int64_t,3>>,7> & rules) {
    int64_t no1 = seed;
    int64_t no2;
    //cout << no1 ;
    for(auto rule : rules) {
        no2 = no1;
        for(auto row : rule) {
            int64_t m1 = row.at(1);
            int64_t m2 = m1 + row.at(2);
            if(no1 >= m1 && no1 <= m2) {
                int64_t diff = no1 - m1;
                no2 = row.at(0) + diff;
                break;
            }
        }
        //cout << " -> " << no2 ;
        no1 = no2;
        //cout << no2 << endl;
    }
    //cout << "\n";
    return no2;
}

void part1(const string & fn) {
    const auto [seeds,rules] = parse(fn);
    int64_t ans = 0;
    vector<int64_t> locs;
    for(int64_t seed : seeds) {
        int64_t location = get_loc(seed,rules);
        locs.push_back(location);
        //cout << seed << " -> " << location << endl;
    }
    ans = *min_element(locs.begin(),locs.end());

    //for(auto [s1,s2] : ip) {}
    
    cout << fn << " day 5 part 1 answer = " << ans << endl;

    for(int64_t i = 0; i < 100; i++) {
    //    cout << i << " -> " << get_loc(i,rules) - i << endl;
    }
}

bool overlap(int64_t i1,int64_t i2,int64_t j1,int64_t j2 ) {
    return i1 <= j2 && i2 >= j1;
    //return j1 >= i1 && j1 <= i2 || j2 >= i1 && j2 <= i2;
}


int main() {
    part1("test_input");
    part1("input");
   // part2("test_input");
    //part2("input");

    
    return 0;
}
