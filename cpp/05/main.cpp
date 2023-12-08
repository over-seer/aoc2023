#include "aoc.h"

#define BIG_NUM 9000000000000000000

using namespace std;

auto cmp = [](const array<int64_t,3> & a1, const array<int64_t,3> & a2) {return a1[1]<a2[1];};

auto parse(const string & fn) {
    const auto ip = read_paragraph_ip(fn);
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
        auto & ref = result.second.at(i-1);
        sort(ref.begin(),ref.end(),cmp);
        if(ref.begin()->at(1) != 0) ref.insert(ref.begin(),{0,0,ref.begin()->at(1)});
        ref.push_back({ref.back().at(1),ref.back().at(1),BIG_NUM});
    }
    return result;
}

int64_t fwd_map(const vector<array<int64_t,3>> & rule, int64_t i) {
    for(auto row : rule) {
        int64_t m1 = row.at(1);
        int64_t m2 = m1 + row.at(2);
        if(i >= m1 && i < m2) {
            int64_t diff = i - m1;
            return row.at(0) + diff;
        }
    }
    return i;
}

int64_t bck_map(const vector<array<int64_t,3>> & rule, int64_t i) {
    for(auto row : rule) {
        int64_t m1 = row.at(0);
        int64_t m2 = m1 + row.at(2);
        if(i >= m1 && i < m2) {
            int64_t diff = i - m1;
            return row.at(1) + diff;
        }
    }
    return i;
}

int64_t get_loc(int64_t seed, const array<vector<array<int64_t,3>>,7> & rules) {
    int64_t no1 = seed;
    for(auto rule : rules) {
        no1 = fwd_map(rule,no1);
    }
    return no1;
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
    
    cout << fn << " day 5 part 1 answer = " << ans << endl;
}

vector<array<int64_t,3>> merge(const array<vector<array<int64_t,3>>,7> & rules) {
    vector<array<int64_t,3>> result;
    set<int64_t> bnds;
    for(size_t i = 0; i < 7; i++) {
        auto & rule = rules[i];
        for(auto & row : rule) {
            auto vj1 = row.at(1);
            auto vj2 = row.at(1) + row.at(2);
            for(size_t j = i; j > 0; j--) {
                vj1 = bck_map(rules[j-1],vj1);
                vj2 = bck_map(rules[j-1],vj2);
            }
            bnds.insert(vj1);
            bnds.insert(vj2);
        }
    }    
    
    for(int64_t i : bnds) result.push_back({get_loc(i,rules), i, 0});

    for(size_t i = 1; i<result.size(); i++) {
        result.at(i-1).at(2) = result.at(i).at(1) - result.at(i-1).at(1);
    }
    result.back().at(2) = BIG_NUM;
    return result;
}

void part2(const string & fn) {
    const auto [seeds,rules] = parse(fn);
    auto merged = merge(rules);

    //quick test to show merged map works
    for(int64_t seed : seeds) {
        int64_t location = fwd_map(merged,seed);
        assert(location == get_loc(seed,rules));
    }
  
    vector<int64_t> locs;
    for(int64_t i = 0; i < seeds.size(); i+=2) {
        auto s1 = seeds.at(i);
        auto s2 = s1 + seeds.at(i+1) - 1;
        locs.push_back(fwd_map(merged,s1));
        locs.push_back(fwd_map(merged,s2));
        for(auto [i0,i1,i2] : merged) {
            if(i1 > s1 && i1 <= s2) locs.push_back(fwd_map(merged,i1));
        }
    }  
    int64_t ans = *min_element(locs.begin(),locs.end());
    cout << fn << " day 5 part 2 answer = " << ans << endl; 
}

int main() {
    part1("test_input");
    part1("input");
    part2("test_input");
    part2("input");
    return 0;
}
