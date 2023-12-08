#include "aoc.h"

using namespace std;

//global strengths so we can change them part 2
map<char,int> strength = 
    {{'2',2},{'3',3},{'4',4},{'5',5},{'6',6},{'7',7},{'8',8},{'9',9},
    {'T',10},{'J',11},{'Q',12},{'K',13},{'A',14}};

struct Hand {
    string cards; // used for highest card scores
    string best_cards; // used to determine type
    int score;
    map<char,int> cton;   // number by card
    map<int,string> ntoc; // cards by number
    int type; 

    Hand(const string & s1, const string & s2, int i) : cards(s1), best_cards(s2), score(i) {
        cton['J'] = 0;
        for(char c : best_cards) {
            if(auto it = cton.find(c); it == cton.end()) {
                cton[c] = 1;
            } else {
                ++cton[c];
            }
        }
        for(auto [c,n] : cton) {
            ntoc[n].push_back(c);
        }
        type = what_type(best_cards);
    }

    int what_type(const string & s) {
        int t;
        int n1 = ntoc.rbegin()->first;
        if(n1 == 5) t = 6;       //5 of a kind
        else if(n1 == 4) t = 5;  //4 of a kind
        else if(n1 == 3 && ntoc.contains(2)) t = 4; //full house
        else if(n1 == 3) t = 3; //3 of a kind
        else if(n1 == 2 && ntoc[2].size() == 2) t = 2; //2 pairs
        else if(n1 == 2) t = 1; //pair
        else t = 0; //high card
        return t;
    }

    char best_not_wild(const string & s) {
        int i = 0;
        char cbest = 'x';
        for(char c : s) {
            if(c != 'J' && strength.at(c) > i) {
                i = strength.at(c);
                cbest = c;
            }
        }
        return cbest;
    }

    //this is overcomplicated - it doesn't matter what the
    //cards are this isn't real poker
    Hand best() {
        string s = cards;
        char cnew;
        int nj = cton.at('J');
        if(nj == 5) {
            cnew = 'A';
        } else if(nj == 4) {
            cnew = ntoc.at(1).at(0);
        } else if(nj == 3) {
            int i = 0;
            cnew = best_not_wild(cards);
        } else if(nj == 2) {
            if(ntoc.at(2).size() == 2) {
                cnew = best_not_wild(ntoc.at(2));
            } else {
                cnew = best_not_wild(cards);
            }
        } else if(nj == 1) {
            if(ntoc.contains(3) && ntoc.at(3).size() == 1) {
                cnew = ntoc.at(3).at(0);
            } else if(ntoc.contains(2) && ntoc.at(2).size() == 1) {
                cnew = ntoc.at(2).at(0);
            } else {
                cnew = best_not_wild(cards);
            }
        }
        replace(s.begin(),s.end(),'J',cnew);
        return Hand(cards,s,score);
    }
};

bool operator<(const Hand & a, const Hand & b) {
    if(a.type < b.type) return true;
    else if(a.type == b.type) {
        for(size_t i : {0,1,2,3,4}) {
            int na = strength.at(a.cards.at(i));
            int nb = strength.at(b.cards.at(i));
            if(na < nb) return true;
            else if(na > nb) return false;
        }
    }
    return false;
}

vector<Hand> parse(string fn) {
    auto ip = read_ip(fn);
    vector<Hand> result;
    for(string & line : ip) {
        auto [s1,s2] = splitn<2>(line);
        result.emplace_back(s1,s1,stoi(s2));
    }
    return result;
}

void part1(const string & fn) {
    auto hands = parse(fn);
    sort(hands.begin(),hands.end());
    int ans = 0;
    for(size_t i = 0; i<hands.size(); i++) {
        //cout << i + 1 << " : " << hands.at(i).cards << " " << hands.at(i).score << endl;
        ans += hands.at(i).score * (i+1);
    }
    cout << fn << " day 7 part 1 answer = " << ans << endl; 
}

void part2(const string & fn) {
    strength['J'] = 0;
    auto hands = parse(fn);
    for(Hand & h : hands) h = h.best();
    sort(hands.begin(),hands.end());
    int ans = 0;
    for(size_t i = 0; i<hands.size(); i++) {
        ans += hands.at(i).score * (i+1);
    }
    cout << fn << " day 7 part 2 answer = " << ans << endl; 
}

int main() {
    part1("test_input");
    part1("input");
    part2("test_input");
    part2("input");
    return 0;
}
