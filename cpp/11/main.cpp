#include "aoc.h"

using namespace std;

auto parse(const string & fn) {
    auto ip = read_ip(fn);
    size_t nrow = ip.size();
    size_t ncol = ip.at(0).size();
    set<array<size_t,2>> pts;
    for(size_t i = 0; i < nrow; i++) {
        for(size_t j = 0; j < ncol; j++) {
            if(ip[i][j] == '#') pts.insert({i,j});
        }
    }
    return make_tuple(nrow,ncol,pts);
}

auto spaces(const set<array<size_t,2>> & ip, size_t nrow, size_t ncol) {
    set<size_t> rows, cols;

    for(size_t row = 0; row < nrow; row++) {
        bool is_blank = true;
        for(size_t col = 0; col < ncol; col++) {
            if(ip.contains({row,col})) {
                is_blank = false;
                break;
            }
        }
        if(is_blank) {
            //cout << "blank row " << row << endl;
            rows.insert(row);
        }
    }


    for(size_t col = 0; col < ncol; col++) {
        bool is_blank = true;
        for(size_t row = 0; row < nrow; row++) {
            if(ip.contains({row,col})) {
                is_blank = false;
                break;
            }
        }
        if(is_blank) {
            //cout << "blank col " << col << endl;
            cols.insert(col);
        }
    }
    return make_pair(rows,cols);
}

size_t actual_pos(size_t x0, set<size_t> blanks, size_t off) {
    size_t x1 = x0;
    for(size_t nxt : blanks) {
        if(x0 > nxt) {
            x1 += off;
        }
    }
    return x1;
}

array<size_t,2> actual_pt(const array<size_t,2> & pt, set<size_t> rows, set<size_t> cols, size_t off) {
    auto [r0,c0] = pt;
    return {actual_pos(r0,rows,off),actual_pos(c0,cols,off)};
}

size_t abs_diff(size_t a, size_t b) {
    return a > b ? a - b : b - a;
}

size_t day11(const string & fn, size_t offset) {
    auto [nrow,ncol,ip] = parse(fn);
    auto [empty_rows,empty_cols] = spaces(ip,nrow,ncol);
    set<array<size_t,2>> actual;
    for(auto pt : ip) {
        actual.insert(actual_pt(pt,empty_rows,empty_cols,offset));
    }
    size_t ans = 0;
    for(auto it1 = actual.begin(); it1 != actual.end(); ++it1) {
        auto [r0,c0] = *it1;
        auto it2 = it1;
        ++it2;
        for(; it2 != actual.end(); ++it2) {
            auto [r1,c1] = *it2;
            size_t md = abs_diff(r1,r0) + abs_diff(c1,c0);
            //cout << diff << endl;
            ans += md;
        }
    }
    return ans;
}

int main() {
    //cout << "test_input " << " day 11 part 1 answer = " << day11("test_input",1) << endl;
    cout << "input " << " day 11 part 1 answer = " << day11("input",1) << endl;
    cout << "input " << " day 11 part 2 answer = " << day11("input",999999) << endl;
    return 0;
}
