#include "aoc.h"

using namespace std;

auto parse_set(const string & s) {
    set<size_t> result;
    stringstream ss(s);
    size_t n;
    while(ss >> n) {
        result.insert(n);
    }
    return result;
}

auto parse(const string & fn) {
    vector<pair<set<size_t>,set<size_t>>> result;
    auto ip = read_ip(fn);
    for(string line : ip) {
        string game = trim_copy(split(line,':')[1]);
        result.push_back({parse_set(trim_copy(split(game,'|')[0])),
        parse_set(trim_copy(split(game,'|')[1]))});
    }
    return result;
}


size_t num_matches(const set<size_t> & s1, const set<size_t> & s2) {
    set<size_t> is;
    set_intersection(s1.begin(),s1.end(),s2.begin(),s2.end(),inserter(is,is.begin()));
    return is.size();
}

void part1(const string & fn) {
    const auto ip = parse(fn);
    int ans = 0;

    for(auto [s1,s2] : ip) {
        size_t n = num_matches(s1,s2);
        if(n > 0) ans += pow(2,n-1);
    }
    
    
    cout << fn << " day 4 part 1 answer = " << ans << endl;
}

class Scratch {
public:
    Scratch(const vector<pair<set<size_t>,set<size_t>>> & c)
     :  cards(c), 
        win(c.size(),0)
    {
        size_t ngames = c.size();
        size_t i = ngames - 1;
        size_t rem = 0;
        while(i > 0) {
            i--;
            rem++;
            size_t nmatches = num_matches(cards.at(i).first,cards.at(i).second);
            size_t won0 = min(nmatches,rem);
            size_t won = won0;
            for(size_t ii = 0; ii < won0; ii++) {
                won += win.at(i + 1 + ii);
            }
            win[i] = won;
        }
    }
    size_t total() {
        size_t sum = cards.size();
        for(size_t i = 0; i < cards.size(); i++) {
            sum += win.at(i);
        }
        return sum;
    }
    vector<pair<set<size_t>,set<size_t>>> cards;
    vector<size_t> win;
};

void part2(const string & fn) {
    Scratch scratch(parse(fn));
    cout << fn << " day 4 part 2 answer = " << scratch.total() << endl;
}

int main() {
    //part1("test_input");
    part1("input");
    //part2("test_input");
    part2("input");
    return 0;
}
