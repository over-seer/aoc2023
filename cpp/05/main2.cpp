#include "aoc.h"
#include "n_array_impl.h"

using namespace std;
using namespace n_array;

auto parse(const string & fn) {
    const auto ip = read_paragraph_ip(fn);
    //vector<size_t> result;
    //map<> result;
    pair<
        vector<size_t>,
        array<set<array<size_t,3>>,7>
        > result;
    for(auto & line : ip.at(0)) {
        auto words = split(line,' ');
        for(size_t i = 1; i < words.size(); i++) {
            //cout << words.at(i) << endl;
            result.first.push_back(stoul(words.at(i)));
        }
    }
    for(size_t i = 1; i < 8; i++) {
        const vector<string> & para = ip.at(i);
        for(size_t j = 1; j < para.size(); j++) {
            const string & line = para.at(j);
            //cout << line << endl;
            auto words = split(line,' ');
            array<size_t,3> nos;
            for(size_t k = 0; k < 3; k++) {
                nos.at(k) = stoul(words.at(k));
            }
            result.second.at(i-1).insert(nos);
        }
        auto & rows = result.second.at(i-1);
        for(auto & row : result.second.at(i-1)) {
            cout << row[0] << " " << row[1] << " " << row[2] << endl;
        }
        cout << endl;
        if(rows.begin()->at(0) != 0) rows.insert({0,0,rows.begin()->at(0)});
        for(auto it = rows.begin(); it != rows.end(); ++it) {
            auto it2 = it;
            it2++;
            if(it2 != rows.end()) {
                size_t nxt = it->at(0) + it->at(2);
                if(it2->at(0) != nxt)
                    rows.insert({nxt,nxt,it2->at(0)-it->at(0)});
            }
        }

        for(auto & row : result.second.at(i-1)) {
            cout << row[0] << " " << row[1] << " " << row[2] << endl;
        }
        cout << endl;
    }
    return result;
}

size_t get_loc(size_t seed, const array<set<array<size_t,3>>,7> & rules) {
    size_t no1 = seed;
    size_t no2;
    //cout << no1 ;
    for(auto rule : rules) {
        no2 = no1;
        for(auto row : rule) {
            size_t m1 = row.at(1);
            size_t m2 = m1 + row.at(2);
            if(no1 >= m1 && no1 <= m2) {
                size_t diff = no1 - m1;
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
    size_t ans = 0;
    vector<size_t> locs;
    for(size_t seed : seeds) {
        size_t location = get_loc(seed,rules);
        locs.push_back(location);
        cout << seed << " -> " << location << endl;
    }
    ans = *min_element(locs.begin(),locs.end());

    //for(auto [s1,s2] : ip) {}
    
    cout << fn << " day 5 part 1 answer = " << ans << endl;
}

bool overlap(size_t i1,size_t i2,size_t j1,size_t j2 ) {
    return i1 <= j2 && i2 >= j1;
    //return j1 >= i1 && j1 <= i2 || j2 >= i1 && j2 <= i2;
}

class Rules {
public:
    Rules(){}



    pair<size_t,size_t> minrange() {
        pair<size_t,size_t> range = {0,1000000000000};
        for(size_t i : {6,5,4,3,2,1,0}) {
            range = irange(i,range);
        }
        return range;
    }

    pair<size_t,size_t> irange(size_t i, pair<size_t,size_t> range) {
        const set<array<size_t,3>> & rows = rules.at(i);
        for(auto row : rows) {
            size_t m1 = row.at(1);
            size_t m2 = row.at(1) + row.at(2);
            if(overlap(m1,m2,range.first,range.second)) {
                size_t n1 = max(m1,range.first);
                size_t n2 = min(m2,range.second);
                //todo?
                return {n1,n2};
            }
        }
        return range;
    }

    vector<pair<size_t,size_t>> minranges() {
        vector<pair<size_t,size_t>> result;
        for(auto row : this->rules[6]) {
            pair<size_t,size_t> range = {row.at(1),row.at(1)+row.at(2)};
            for(size_t i : {5,4,3,2,1,0}) {
                result.push_back(irange(i,range));
            }
        }
        return result;
    }

    vector<size_t> seeds;
    array<set<array<size_t,3>>,7> rules;
};






void part2(const string & fn) {
    const auto [seeds,r] = parse(fn);
    size_t ans = 0;
    Rules rules;
    rules.seeds = seeds;
    rules.rules = r;
    vector<size_t> locs;
    auto min_range = rules.minrange();
    cout << "identified min range " << min_range.first << " " << min_range.second 
    << " " << get_loc(min_range.first,r) << " " << get_loc(min_range.second,r) << endl;
    for(size_t i = 0; i < rules.seeds.size(); i += 2) {
        //pair<size_t,size_t> seed_range = {rules.seeds.at(i),rules.seeds.at(i) + rules.seeds.at(i+1)};
        size_t s1 = rules.seeds.at(i);
        size_t s2 = s1 + rules.seeds.at(i+1);
        cout << s1 << " " << s2<< " actual seed range\n";
        size_t is;
        for(auto min_range : rules.minranges()) {
            if(overlap(s1,s2,min_range.first, min_range.second )) {
                is = max(s1,min_range.first);
                locs.push_back(get_loc(is,rules.rules));
                cout << is << " -> " << get_loc(s1,rules.rules) << endl;
                break;
            }
        }
    }
    ans = *min_element(locs.begin(),locs.end());

    //for(auto [s1,s2] : ip) {}
    
    cout << fn << " day 5 part 2 answer = " << ans << endl;
}

int main() {
    part1("test_input");
    //part1("input");
    part2("test_input");
    //part2("input");
    
    return 0;
}
