#include "aoc.h"

using namespace std;

auto parse(const string & fn) {
    const auto ip = read_paragraph_ip(fn);
    pair<
        vector<int64_t>,
        array<vector<array<int64_t,3>>,7>
        > result;
    for(auto & line : ip.at(0)) {
        auto words = split(line,' ');
        for(int64_t i = 1; i < words.size(); i++) {
            result.first.push_back(stoul(words.at(i)));
        }
    }
    for(int64_t i = 1; i < 8; i++) {
        const vector<string> & para = ip.at(i);
        for(int64_t j = 1; j < para.size(); j++) {
            const string & line = para.at(j);
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

auto reformat(vector<array<int64_t,3>> ip) {
    for(auto & row : ip) swap(row.at(0),row.at(1));
    sort(ip.begin(),ip.end());
    if(ip.begin()->at(0) != 0) ip.insert(ip.begin(),{0,0,ip.begin()->at(0)});
    //for(auto line : ip) cout << line[0] <<" "<< line[1] <<" "<< line[2] << endl;cout << endl;// exit(1);
    map<int64_t,int64_t> op;
    int64_t nxt = 0;
    for(auto row : ip) {
        if(nxt != row[0]) {cout << "error"<<endl;exit(1);}
        int64_t diff = row[1] - row[0];
        nxt = row[0] + row[2];
        op.insert({nxt,diff});
    }
    //op.insert({BIG_int64_t,0});    
    return op;
}

auto reformat(const array<vector<array<int64_t,3>>,7> & ip) {
    array<map<int64_t,int64_t>,7> op;
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

int64_t get_loc(const array<map<int64_t,int64_t>,7> & r, int64_t i) {
    for(auto row : r) i = mapping(row,i);
    return i;
}

auto merge(const map<int64_t,int64_t> & a, const map<int64_t,int64_t> & b) {
    map<int64_t,int64_t> c;
    int64_t ialast = 0;
    int64_t iblast = ialast + a.begin()->second;
    for(auto rowa : a) {
        int64_t ia = rowa.first;
        int64_t da = rowa.second;
        c.insert({ia, da + diff(b, ia + da)});
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
        result = merge(result,r.at(i));
        for(auto row : result) cout << "merge " << i << " " << row.first <<" "<<row.second <<endl;
    }
    //exit(1);
    return result;
}

void part1(const string & fn) {
    auto [seeds,ip] = parse(fn);
    auto rules = reformat(ip);
    for(auto rows : rules) {
        //for(auto row : rows) cout << row.first << " " << row.second << endl;
        //cout << endl;
    }
    auto merged = merge(rules);
    vector<int64_t> locs;
    for(auto i : seeds) {
        auto loc1 = get_loc(rules,i);
        auto loc2 =  mapping(merged,i);
        cout << i << " -> " << loc1 <<" "<< loc2 << endl;
        locs.push_back(loc2);
    }
    cout << fn << " day 5 part 1 answer = " << *min_element(locs.begin(),locs.end()) << endl;
    for(int64_t i = 0; i < 102; i++) {
        cout << i << " -> " << get_loc(rules,i) - i <<" "<< mapping(merged,i) - i << endl;
    }
}

int main() {
    part1("test_input");
    //part1("input");
   // part2("test_input");
    //part2("input");

    
    return 0;
}
