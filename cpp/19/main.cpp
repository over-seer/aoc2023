#include "aoc.h"

using namespace std;

enum Op{GOTO,GT,LT};
enum XMAS{X,M,A,S};

struct Rule {
    XMAS xmas;
    Op op = GOTO;
    int64_t num = 0;
    string dest;
};

Rule parse_rule(string s) {
    Rule rule;
    if(auto v = split(s,':'); v.size() == 1) {
        //cout << s << endl;
        rule.dest = v.front();
    } else {
        rule.dest = v.back();
        string s1 = v.front();
        char c =  s1.at(0);
        switch(c) {
            case 'x': rule.xmas = X; break;
            case 'm': rule.xmas = M; break;
            case 'a': rule.xmas = A; break;
            case 's': rule.xmas = S; break;
            default : 
                cout << c << endl;
                throw runtime_error("bad XMAS");
        }
        char cop = s.at(1);
        if(cop == '<') rule.op = LT;
        else if (cop == '>' ) rule.op = GT;
        else throw runtime_error("bad < > char!");
        s1 = s1.substr(2);
        rule.num = stoi(s1);
    }
    return rule;
}

auto parse_workflow(const string & s) {
    auto v = split(s,'{');
    string key = v.at(0);
    string remainder = v.at(1);
    remainder.pop_back();
    auto srules = split(remainder,',');
    vector<Rule> rules;
    for(auto s : srules) {
        rules.push_back(parse_rule(s));
    }
    return make_pair(key,rules);  
}

auto parse(const string & fn) {
    auto ip = read_paragraph_ip(fn);
    map<string,vector<Rule>> rules;
    for(auto s : ip.at(0)) {
        rules.insert(parse_workflow(s));
    }
    auto srats = ip.at(1);
    vector<array<int64_t,4>> ratings;
    for(string s : srats) {
        array<int64_t,4> xmas;
        s = s.substr(1); // pop front
        s.pop_back();
        replace(s.begin(),s.end(),',',' ');
        auto sxmas = splitn<4>(s);
        for(size_t i : {0,1,2,3}) {
            string si = sxmas.at(i);
            si = si.substr(2);
            xmas.at(i) = stoi(si);
        }
        ratings.push_back(xmas);
    }
    return make_pair(rules,ratings);
}

bool workflow_accept(   const map<string,vector<Rule>> & rules,
                        const array<int64_t,4> & xmas,
                        const string & key) {
    //cout << key << endl;
    if(key == "A") return true;
    if(key == "R") return false;
    auto & wf = rules.at(key);
    for(auto rule : wf) {
        switch (rule.op) {
            case LT:
                if(xmas.at(rule.xmas) < rule.num) {
                    return workflow_accept(rules,xmas,rule.dest);
                }
                break;
            case GT:
                if(xmas.at(rule.xmas) > rule.num) {
                    return workflow_accept(rules,xmas,rule.dest);
                }
                break;
            case GOTO:
                return workflow_accept(rules,xmas,rule.dest);
            default: 
                cout << rule.op << endl;
                throw runtime_error("bad rule!");
        }
    }
    throw runtime_error("neither accepted nor rejected!");
}

struct Range {
    int64_t gt = 0;
    int64_t lt = 4001;
};

void workflow_ranges(   vector<array<Range,4>> & xmas_ranges,
                        const map<string,vector<Rule>> & rules,
                        array<Range,4> xmas_range,
                        const string & key)
{
    if(key == "A") {
        xmas_ranges.push_back(xmas_range);
        return;
    }
    else if(key == "R") return;
    else {
        auto & wf = rules.at(key);
        for(auto rule : wf) {
            auto xmas_match = xmas_range;
            //auto xmas_nomatch = xmas_range;
            Range & rng_match = xmas_match.at(rule.xmas);
            Range & rng_nomatch = xmas_range.at(rule.xmas);
            switch (rule.op) {
                case LT:
                    rng_match.lt = min(rng_match.lt,rule.num);
                    rng_nomatch.gt = max(rng_nomatch.gt,rule.num - 1);
                    workflow_ranges(xmas_ranges,rules,xmas_match,rule.dest);
                    break;
                case GT:
                    rng_match.gt = max(rng_match.gt,rule.num);
                    rng_nomatch.lt = min(rng_nomatch.lt,rule.num + 1);
                    workflow_ranges(xmas_ranges,rules,xmas_match,rule.dest);
                    break;
                case GOTO:
                    workflow_ranges(xmas_ranges,rules,xmas_range,rule.dest);
                    break;
            }
        }        
    }
}

void part1(const string & fn) {
    auto [rules,ratings] = parse(fn);
    set<array<int64_t,4>> aset, rset;
    for(auto xmas : ratings) {
        if(workflow_accept(rules,xmas,"in")) {
            aset.insert(xmas);
        }
    }
    int64_t ans1 = 0;
    for(auto [x,m,a,s] : aset) {
        ans1 += (x+m+a+s);
    }
    cout << fn << " day 19 part 1 answer = " << ans1 << endl;
}

void part2(const string & fn) {
    auto [rules,ratings] = parse(fn);
    vector<array<Range,4>> ranges;
    workflow_ranges(ranges,rules,array<Range,4>(),"in");
    int64_t ans2 = 0;
    for(auto xmas : ranges) {
        auto [x,m,a,s] = xmas;
        //cout << "x " <<  x.lt - x.gt << ",  m " << m.lt - m.gt << ",  a " << a.lt - a.gt << ",  s " << s.lt - s.gt << endl;
        int64_t prod = 1;
        for(Range r : xmas) {
            if(r.lt > r.gt + 1) {
                prod *= (r.lt - r.gt - 1);
            }
        }
        ans2 += prod;
    }
    cout << fn << " day 19 part 2 answer = " << ans2 << endl;

}

int main() {
    //part1("test_input");
    part1("input");
    //part2("test_input");
    part2("input");
    return 0;
}
