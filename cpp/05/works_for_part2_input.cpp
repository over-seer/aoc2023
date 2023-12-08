#include "aoc.h"
#include "n_array_impl.h"

#define BIG_NUM 9000000000000000000

using namespace std;
using namespace n_array;


auto cmp = [](const array<int64_t,3> & a1, const array<int64_t,3> & a2) {return a1[1]<a2[1];};

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
        auto & ref = result.second.at(i-1);
        sort(ref.begin(),ref.end(),cmp);
        if(ref.begin()->at(1) != 0) ref.insert(ref.begin(),{0,0,ref.begin()->at(1)});
        ref.push_back({ref.back().at(1),ref.back().at(1),BIG_NUM});
    }
    return result;
}

int64_t diff(const vector<array<int64_t,3>> & rule, int64_t i) {
    for(auto row : rule) {
        int64_t m1 = row.at(1);
        int64_t m2 = m1 + row.at(2);
        if(i >= m1 && i <= m2) {
            return i - m1;
        }
    }
    return i;
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
        if(i >= m1 && i <= m2) {
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
        //cout << no1 << endl;
    }
    return no1;
}

int64_t fwd_map(int64_t x, const array<vector<array<int64_t,3>>,7> & rules, size_t i0, size_t i1) {
    for(size_t i = i0; i < i1; i++) {
        x = fwd_map(rules[i],x);
    }
    return x;
}

int64_t bck_map(int64_t x, const array<vector<array<int64_t,3>>,7> & rules, int64_t i0, int64_t i1) {
    for(int64_t i = i1 - 1; i >= i1; i--) {
        x = bck_map(rules[i],x);
    }
    return x;
}

void part1(const string & fn) {
    const auto [seeds,rules] = parse(fn);
    int64_t ans = 0;
    vector<int64_t> locs;
    for(int64_t seed : seeds) {
        //int64_t location = get_loc(seed,rules);
        int64_t location = fwd_map(seed,rules,0,6);

        locs.push_back(location);
        cout << seed << " -> " << location << endl;
    }
    ans = *min_element(locs.begin(),locs.end());

    //for(auto [s1,s2] : ip) {}
    
    cout << fn << " day 5 part 1 answer = " << ans << endl;

    for(int64_t i = 0; i < 110; i++) {
        //cout << i << " -> " << get_loc(i,rules) - i << endl;
        auto j =fwd_map(rules[0],i);
        auto k =bck_map(rules[0],j);
        //cout << i << " -> " <<  j <<" -> " <<  k << endl;
    }
}

bool overlap(int64_t i1,int64_t i2,int64_t j1,int64_t j2 ) {
    return i1 <= j2 && i2 >= j1;
    //return j1 >= i1 && j1 <= i2 || j2 >= i1 && j2 <= i2;
}

vector<array<int64_t,3>> merge(vector<array<int64_t,3>> a, vector<array<int64_t,3>> b) {
    vector<array<int64_t,3>> c;
    //auto mc = merge(ma,mb);
    set<int64_t> bnds;
    for(auto & ra : a) {
        int64_t m1 = ra.at(0);
        int64_t m2 = m1 + ra.at(2);
        bnds.insert(ra.at(1));
    }

    for(auto & rb : b) {
        int64_t ib = rb.at(1);
        bnds.insert(bck_map(a,ib));
    }

    for(int64_t i : bnds) {
        array<int64_t,3> el = {fwd_map(b,fwd_map(a,i)), i, 0};
        c.push_back(el);
    }
    for(size_t i = 1; i<c.size(); i++) {
        c.at(i-1).at(2) = c.at(i).at(1) - c.at(i-1).at(1);
    }
    c.back().at(2) = BIG_NUM;

    return c;
}

vector<array<int64_t,3>> merge(const array<vector<array<int64_t,3>>,7> & rules) {
    /*
    vector<array<int64_t,3>> result = rules.at(0);
    for(size_t i : {1,2,3,4,5,6})  {
        result = merge(result,rules.at(i));
    }*/

    vector<array<int64_t,3>> result;
    set<int64_t> bnds;
    for(size_t i = 0; i < 7; i++) {
        auto & rule = rules[i];
        for(auto & row : rule) {
            auto vi = row.at(1);
            auto v0 = bck_map(vi,rules,0,i);
            //cout <<  vi << "  " << v0 << endl;
            bnds.insert(v0);
            bnds.insert(bck_map(row.at(1)+row.at(2)-1,rules,0,i));
            bnds.insert(bck_map(row.at(1)+row.at(2),rules,0,i));
        }
    }    
    
    for(int64_t i : bnds) {
        //cout << i << endl;
        array<int64_t,3> el = {get_loc(i,rules), i, 0};
        result.push_back(el);
    }
    for(size_t i = 1; i<result.size(); i++) {
        result.at(i-1).at(2) = result.at(i).at(1) - result.at(i-1).at(1);
    }
    result.back().at(2) = BIG_NUM;
    //for(auto [i0,i1,i2] : result) cout << i0 <<" , "<< i1 <<" , "<< i2 << endl;
    return result;
}

void part2(const string & fn) {
    const auto [seeds,rules] = parse(fn);
    auto merged = merge(rules);

    vector<int64_t> locs;
    for(int64_t seed : seeds) {
        int64_t location = fwd_map(merged,seed);
        locs.push_back(location);
        cout << seed << " -> " << location << endl;
    }
    int64_t ans = *min_element(locs.begin(),locs.end());
    
    cout << fn << " day 5 part 1 answer = " << ans << endl;  
    locs.clear();
    for(int64_t i = 0; i < seeds.size(); i+=2) {
        auto s1 = seeds.at(i);
        auto s2 = s1 + seeds.at(i+1) - 1;
        locs.push_back(fwd_map(merged,s1));
        locs.push_back(fwd_map(merged,s2));
        for(auto [i0,i1,i2] : merged) {
            if(overlap(s1,s2,i1,i1+i2-1)) locs.push_back(fwd_map(merged,i1));
        }
    }  
    cout << fn << " day 5 part 2 answer = " << *min_element(locs.begin(),locs.end()) << endl; 
    cout << fwd_map(merged,82) << endl;
    for(int64_t i = 0; i < 100; i++) {
        //cout << i << " -> " << fwd_map(merged,i) - i << endl;
    }
}

void test() {
    auto [seeds,rules] = parse("test_input");
    //cout << get_loc(55,rules) << endl;
    cout << get_loc(13,rules) << endl;
    cout << get_loc(14,rules) << endl;
    //auto merged = merge(rules);
    //cout << fwd_map(merged,55) << endl;
    for(int64_t i = 0; i < 110; i++) {
        //cout << i << " " << get_loc(i,rules) << " " << fwd_map(merged,i) << endl;
    }
}

int main() {
    part1("test_input");
    //part1("input");
    part2("test_input");
    part2("input");

    test();
    return 0;
}
