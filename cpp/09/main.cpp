#include "aoc.h"

using namespace std;

auto parse(const string & fn) {
    auto lines = read_ip(fn);
    vector<vector<int64_t>> result;
    for(string line : lines) {
        auto words = split(line,' ');
        vector<int64_t> nos;
        for(string word : words) {
            nos.push_back(stoi(word));
        }
        result.push_back(nos);
    }
    return result;
}

array<int64_t,2> next(vector<int64_t> & row0) {
    vector<vector<int64_t>> rows(row0.size());
    rows[0] = row0;
    for(size_t irow = 1; irow < row0.size(); irow++) {
        auto & rowref = rows[irow-1];
        auto & diffs = rows[irow];
        for(size_t i = 1; i < rowref.size(); i++) {
            int64_t diff = rowref.at(i) - rowref.at(i-1);
            diffs.push_back(diff);
        }
        int64_t result0 = diffs.front();
        int64_t result1 = diffs.back();
        if(all_of(diffs.begin(),diffs.end(),[&](int64_t x){return x == result0;})) {
            for(size_t jrow = irow; jrow != 0; jrow--) {
                result1 = rows.at(jrow - 1).back() + result1;
                result0 = rows.at(jrow - 1).front() - result0;
            }
            return {result0,result1};
        }
    }
    return {0,0};
}

void day9(const string & fn) {
    auto ip = parse(fn);
    int64_t ans1 = 0;
    int64_t ans2 = 0;
    for(auto line : ip) {
        auto [lst,nxt] = next(line);
        ans1 += nxt;
        ans2 += lst;
    }

    cout << fn << " day 9 part 1 answer = " << ans1 << endl; 
    cout << fn << " day 9 part 2 answer = " << ans2 << endl; 
}

int main() {
    //day9("test_input");
    day9("input");
    return 0;
}
