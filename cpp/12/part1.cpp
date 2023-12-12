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
    /*
    for(auto [s1,nos] : result) {
        cout << s1 << "    ";
        for(auto n : nos) cout << n << ","; 
        cout << endl;
     }*/
    return result;
}

size_t min_len(const vector<size_t> & nos) {
    return nos.empty() ? 0 : accumulate(nos.begin(),nos.end(),nos.size()-1);
}

void iterate(size_t d, size_t nd, vector<size_t> n, vector<size_t> res) {
    if (d >= nd)  return;

    for (size_t i = 0; i < nd; i++) { 
        res[d] = i;
        iterate(d+1,nd,n,res);
    }
}

auto combs(size_t len, const vector<size_t> & nos) {
    size_t minl = min_len(nos);
    size_t rem = len - minl;
    vector<size_t> gaps(nos.size() + 1,0);
    //vector<size_t> spans(nos.size()*2 + 1,0);
    size_t irem = rem;
    for(size_t igap = 0; igap < gaps.size() - 1; igap++) {
        for(size_t i = 0; i <= rem ; i++) {

        }
        irem--;
    }
    if(len == minl) return 1;
    return 0;
}

auto ncombs(const string & s, const vector<size_t> & nos) {
    size_t minl = min_len(nos);
    if(s.size() == minl) return 1;
    return 0;
}

string regex_form(const vector<size_t> & nos) {
    string s = "x*";
    for(auto n : nos) {
        for(auto i =0; i < n; i++) {
            s.push_back('#');
        }
        s+="x+";
    }
    s.back() = '*';
    return s;
}


void part1(const string & fn) {
    auto ip = parse(fn);
    size_t maxq = 0;
    size_t maxd = 0;
    size_t ncombs = 0;

    size_t ans = 0;
    for(auto [s,nos] : ip) {
        string rule = regex_form(nos);
        //cout << rule << endl;
        size_t nq = std::count_if(s.begin(),s.end(),[](char c){return c =='?';}); 
        ncombs += pow(2,nq);
        size_t d = s.size() - min_len(nos);
        if(d > maxd) {
            //cout << d << " = " << s.size() << " - " << min_len(nos) << endl;
            //cout << s << " " << rule << endl;
            maxd = d;
        }
        //maxd = max(maxd,s.size() - min_len(nos));
        maxq = max(maxq,nq);

        for(size_t i = 0; i < pow(2,nq); i++) {
            bitset<32> b(i);
            string s1 = s;
            for(size_t ii = 0; ii < nq; ii++) {
                char c = b[ii] ? '#' : 'x';
                s1[s1.find('?')] = c;
            }
            if(regex_match(s1,regex(rule))) {
                //cout << s1 << " " << rule << endl;          
                ans++;
            }
        }
    }
    //cout << "max num ? = " << maxq << endl;
    //cout << "ncombs ? = " << ncombs << endl;
    //cout << "max length diff = " << maxd << endl;
    cout << fn << " day 12 part 1 answer = " << ans << endl;
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
    //cout << "it fits!" <<endl;

    if(s[i] == '#') return nullopt;

    //block of size n fits - now look for a possible gap
    while(i < s.size()) {
        //cout << "looking for a possible gap... " << i << endl;
        if(s[i] != '#') {
            break;
        }
        i++;
    }

    while(i < s.size()) {
        i++;
        //cout << "looking for a possible block... " << i << endl;
        if(s[i] != 'x') {
            break;
        }
    }

    return {i};
}

vector<string> v;

struct NumCombs {
    string s;
    vector<size_t> nos;
    map<pair<size_t,size_t>,size_t> cache;

    NumCombs(string str, vector<size_t> ns) : s(str), nos(ns) {}
    
    size_t operator()(size_t i0, size_t iblock) {
        //if(auto it = cache.find({i0,iblock}); it != cache.end()) return it->second;
        cout << s << " " << i0 << " nos[" << iblock << "] = " << nos[iblock] << endl;
        if(iblock == nos.size()) {

            if(find(s.begin() + i0, s.end(), '#') == s.end()) {
                cout << s << " " << i0 << endl;
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
            cout << s << " i = " << i << " iblock = " << iblock << " min_len = " << minl << endl;
            if(s.size() - i < minl) {
                break;
            }
            char c = s[i];
            if(c != 'x') {
                if(auto opt = fits(s,i,nos[iblock])) {
                    cout << "it fits!\n";
                    size_t inext = *opt;
                    //cout << inext << endl;
                    size_t next_block = iblock + 1;
                
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
    s.pop_back();
    return make_pair(s5,nos5);
}


void part2(const string & fn) {
    auto ip = parse(fn);

    size_t ans = 0;
    for(auto snos : ip) {
        //snos = unfold(snos);
        auto [s,nos] = snos;
        NumCombs num_combs(s,nos);
        size_t n = num_combs(0,0);
        ans += n;
        cout << s << " " << n << endl;
    }
    //cout << "max num ? = " << maxq << endl;
    //cout << "ncombs ? = " << ncombs << endl;
    //cout << "max length diff = " << maxd << endl;
    cout << fn << " day 12 part 2 answer = " << ans << endl;
}


int main() {
    part1("test_input");
    part1("input");
    part2("test_input");
    part2("input");
    //part2("input");

    return 0;
}
