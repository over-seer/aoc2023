#include "aoc.h"

using namespace std;

using I2 = vector<vector<int>>;

void fillsums(array<I2,12> & sums, int val) {
    for(auto & i2 : sums)
        for(auto & v : i2) 
            for(auto & i : v)
                i = min(i,val);
}

int dir(int dr, int dc) {
    if(dr == -1) return 0;
    if(dr == 1) return 1;
    if(dc == -1) return 2;
    else return 3;
}

int ix(int dr, int dc, int since_corner) {
    return dir(dr,dc) * 3 + since_corner;
}

struct City {
    I2 wts;
    int nrow;
    int ncol;
    int lim;

    City(const I2 & w) 
        : wts(w), nrow(w.size()), ncol(w.at(0).size()) {lim = guess();}

    int guess() {
        int i = 0;
        for(size_t ix = 1; ix < nrow; ix++) {
            i += wts.at(ix).at(ncol - 1) + wts.at(ix).at(ncol -2);
        }
        i = accumulate(wts.at(0).begin(), wts.at(0).end(), i);
        return accumulate(wts.at(1).begin(), wts.at(1).end(), i);
    }

    auto empty_sums() {
        auto i2 = I2(nrow,vector<int>(ncol,guess()+1));
        array<I2,12> sums = {i2,i2,i2,i2,i2,i2,i2,i2,i2,i2,i2,i2};
        //sums.at(0).at(0).at(0) = 0;
        return sums;
    }

    void best_path(array<I2,12> & sums, int since_corner, int sum, int row, int col, int dr, int dc) {
        if(row < 0 || col < 0 || row >= nrow || col >= ncol) return;
        int w = wts.at(row).at(col);
        if(sum >= sums.at(ix(dr,dc,since_corner)).at(row).at(col) || sum >= lim) return;
        if(row == nrow - 1 && col == ncol - 1) {
            // /cout << sum << endl;
            lim = sum;
        }
        sums.at(ix(dr,dc,since_corner)).at(row).at(col) = sum;
        sum += w;
        int dr1 = dc;
        int dr2 = -dc;
        int dc1 = dr;
        int dc2 = -dr;
        if(since_corner < 2) best_path(sums,++since_corner,sum,row+dr,col+dc,dr,dc);
        best_path(sums,0,sum,row+dr2,col+dc2,dr2,dc2);
        best_path(sums,0,sum,row+dr1,col+dc1,dr1,dc1);
    }
};

auto parse(const string & fn) {
    const auto ip = read_ip(fn);
    const int nr = ip.size();
    const int nc = ip.at(0).size();
    I2 wts(nr,vector<int>(nc));
    for(size_t row = 0; row < nr; row++) {
        for(size_t col = 0; col < nc; col++) {
            wts.at(row).at(col) = (int)(ip.at(row).at(col) - '0');
        }
    }
    return wts;    
}

void part1(const string & fn) {
    auto wts = parse(fn);
    City city(wts);
    array<I2,12> sums = city.empty_sums();

    city.best_path(sums,0,0,0,0,0,1);

    int ans1 = city.guess();
    for(auto i2 : sums) ans1 = min(i2.back().back(),ans1);
    ans1 = ans1 - city.wts.front().front() + city.wts.back().back();
    cout << fn << " day 17 part 1 answer = " << ans1 << endl;
}

int main() {
    //part1("test_input");
    part1("input");
    return 0;
}
