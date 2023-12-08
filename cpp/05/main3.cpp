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
        array<set<array<int64_t,3>>,7>
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
            result.second.at(i-1).insert(nos);
        }
        auto & rows = result.second.at(i-1);
        if(rows.begin()->at(0) != 0) rows.insert({0,0,rows.begin()->at(0)});
        /*for(auto it = rows.begin(); it != rows.end(); ++it) {
            auto it2 = it;
            it2++;
            if(it2 != rows.end()) {
                int64_t nxt = it->at(0) + it->at(2);
                if(it2->at(0) != nxt)
                    rows.insert({nxt,nxt,it2->at(0)-it->at(0)});
            }
        }*/
    }
    return result;
}

auto reformat(const set<array<int64_t,3>> & ip) {
    map<int64_t,int64_t> op;
    int64_t nxt;
    for(auto row : ip) {
        int64_t diff = row[0] - row[1];
        nxt = row[1] + row[2];
        op.insert({nxt,diff});
    }
    //op.insert({BIG_int64_t,0});    
    return op;
}

auto reformat(const array<set<array<int64_t,3>>,7> & ip) {
    array<map<int64_t,int64_t>,7> op;
    for(size_t i : {0,1,2,3,4,5,6}) {
        op[i] = reformat(ip[i]);
    }
    return op;
}

auto reformat(const map<int64_t,int64_t> & ip) {
    set<array<int64_t,3>> op;
    int64_t lst = 0;
    for(auto row : ip) {
        int64_t nxt = lst + row.second;
        int64_t tonxt = row.first - lst;
        op.insert({nxt,lst,tonxt});
    }
    //op.insert({BIG_int64_t,0});    
    return op;
}

auto reformat(const array<map<int64_t,int64_t>,7> & ip) {
    array<set<array<int64_t,3>>,7> op;
    for(size_t i : {0,1,2,3,4,5,6}) {
        op[i] = reformat(ip[i]);
    }
    return op;
}

int64_t diff(const map<int64_t,int64_t> & r, int64_t i) {
    for(auto & row : r) {
        if(i < row.first) {
            return row.second;
        }
    }
    return 0;
}

int64_t mapping(const map<int64_t,int64_t> & r, int64_t i) {
    auto j = i + diff(r,i);
    if(j < 0) {
        cout << "error " << i << " -> " << j <<endl;exit(1);
    }
    return i + diff(r,i);
}

void insert_if_bigger(map<int64_t,int64_t> & m, int64_t k, int64_t v) {
    auto it = m.find(k);
    if(it == m.end()) m.insert({k,v});
    else {
        throw runtime_error("key already used");
    }
}

int64_t get_loc(const array<map<int64_t,int64_t>,7> & r, int64_t i) {
    for(auto row : r) i = mapping(row,i);
    return i;
}

int get_loc(const array<set<array<int64_t,3>>,7> & rules, int seed) {
    int no1 = seed;
    int no2;
    //cout << no1 ;
    for(auto rule : rules) {
        no2 = no1;
        for(auto row : rule) {
            int m1 = row.at(1);
            int m2 = m1 + row.at(2);
            if(no1 >= m1 && no1 <= m2) {
                int diff = no1 - m1;
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

auto merge(const map<int64_t,int64_t> & a, const map<int64_t,int64_t> & b) {
    map<int64_t,int64_t> c;
    int64_t ialast = 0;
    int64_t iblast = ialast + a.begin()->second;
    for(auto rowa : a) {
        int64_t ia = rowa.first;
        int64_t da = rowa.second;
        c.insert({ia, da + diff(b, ialast + da)});
        for(auto rowb : b) {
            int64_t ib = rowb.first;
            int64_t db = rowb.second;
            if(ib >= iblast && ib < ia + da) {
                c.insert({ib - da, da + db});
            }
        }
        ialast = ia;
        iblast = ia + da; 
    }
    //for(auto crow : c) cout << crow.first << " (combined) " << crow.second << endl;
    return c;
}

auto merge(const array<map<int64_t,int64_t>,7> & r) {
    map<int64_t,int64_t> result = r.front();
    for(int64_t i : {1,2,3,4,5,6}) {
    //for(int64_t i : {1}){//},2,3,4,5,6}) {
        result = merge(result,r.at(i));
    }
    return result;
}

bool overlap(int64_t i1,int64_t i2,int64_t j1,int64_t j2 ) {
    return i1 <= j2 && i2 >= j1;
    //return j1 >= i1 && j1 <= i2 || j2 >= i1 && j2 <= i2;
}

void part1(const string & fn) {
    const auto [seeds,r] = parse(fn);
    const auto rules = reformat(r);
    auto merged = merge(rules);
    for(size_t i : {0,1,2,3,4,5,6}) {
        for(auto & row : rules[i]) {
            //cout << i << " : " << row.first << ", " << row.second << endl;
        }
    }
    //for(int64_t i=0; i<102; i++ ) cout << i << " -> " << diff(rules[0],i) << endl;
    //exit(1);
    for(auto row : merged) {
       // cout << row.first << " " << row.second << endl;
    }
    vector<int64_t> locs;
    for(int64_t i : seeds) {
        int64_t loc = get_loc(r,i);
        int64_t loc2 = mapping(merged,i);
        int64_t d = diff(merged,i);;
        int64_t loc3 = get_loc(rules,i);
 

        locs.push_back(loc);
        cout << i << " -> " << d << " " << loc-i <<" " << loc2-i << " " <<loc3-i << endl;
    }

    for(int64_t i=0 ; i<101; i++) {
        int64_t loc = get_loc(r,i);
        int64_t loc2 = mapping(merged,i);
        int64_t loc3 = get_loc(rules,i);
        int64_t d = diff(merged,i);;
 

        locs.push_back(loc);
        cout << i <<"    -> " << d <<  "  " << loc <<" " << loc2 << " " <<loc3 << endl;
    }

    cout << fn << " day 5 part 1 answer = " << *min_element(locs.begin(),locs.end()) << endl; 
}



void part2(const string & fn) {
    const auto [seeds,r] = parse(fn);
    const auto rules = reformat(r);
    auto merged = merge(rules);
    for(auto row : merged) {
        cout << row.first << " " << row.second << endl;;
    }


    int64_t ans = 0;


    

    //for(auto [s1,s2] : ip) {}
    
    cout << fn << " day 5 part 2 answer = " << ans << endl;
}

int main() {
    part1("test_input");
    //part1("input");
    //part2("test_input");
    //part2("input");
    
    return 0;
}
