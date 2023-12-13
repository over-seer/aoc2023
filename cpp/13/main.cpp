#include "aoc.h"

using namespace std;

int num_above(const vector<string> & patt) {
    int nr = patt.size();
    for(int r1 = 0; r1 < nr - 1; r1 += 1)
    {
        bool is_match = false;
        if(patt.at(r1) == patt.at(r1+1)) {
            is_match = true;
            for(
                int r2 = r1 - 1, r3 = r1 + 2; 
                r2 >= 0 && r3 < nr && is_match; 
                r2--, r3++
                ) {
                if(patt.at(r2) != patt.at(r3)) {
                    is_match = false;
                    break;
                }
            }
        }

        if(is_match) return r1 + 1;
    }
    return 0;
}

vector<string> transpose(const vector<string> & a) {
    size_t nr = a.size();
    size_t nc = a.at(0).size();
    vector<string> b(nc,string(nr,'_'));
    for(size_t r = 0; r < nr; r++) {
        for(size_t c = 0; c < nc; c++) {
            b.at(c).at(r) = a.at(r).at(c);
        }
    }
    return b;
}

int count_bad(const string & s1, const string & s2) {
    int n = 0;
    if(s1.size() != s2.size()) throw runtime_error("strings don't match!");
    for(size_t i = 0; i < s1.size(); i++) {
        if(s1.at(i) != s2.at(i)) {
            n++;
        }
    }
    return n;
}

int num_above_with_smudge(const vector<string> & patt) {
    int nr = patt.size();
    int nc = patt.at(0).size();

    for(int r1 = 0; r1 < nr - 1; r1 += 1)
    {
        int num_bad = count_bad(patt.at(r1),patt.at(r1+1));
        if(num_bad < 2) {
            for(
                int r2 = r1 - 1, r3 = r1 + 2; 
                r2 >= 0 && r3 < nr; 
                r2--, r3++
                ) {
                for(int c = 0; c < nc; c++) {
                    if(patt.at(r2).at(c) != patt.at(r3).at(c)) {
                        num_bad++;
                    }
                }
            }
            if(num_bad == 1) return r1 + 1;
        }
    }
    return 0;
}

void day13(const string & fn) {
    auto ip = read_paragraph_ip(fn);
    int ans1 = 0;
    int ans2 = 0;
    for(auto patt1 : ip)
    {
        auto patt2 = transpose(patt1);
        int r1 = num_above(patt1);
        int c1 = num_above(patt2);
        ans1 += 100 * r1 + c1;
        int r2 = num_above_with_smudge(patt1);
        int c2 = num_above_with_smudge(patt2);
        ans2 += 100 * r2 + c2;
    }
    cout << fn << " day 13 part 1 answer = " << ans1 << endl;
    cout << fn << " day 13 part 2 answer = " << ans2 << endl;
}

int main() {
    //day13("test_input");
    day13("input");
    return 0;
}
