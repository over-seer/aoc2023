#include "aoc.h"

using namespace std;

using I2 = vector<vector<int>>;

void fillsums(array<I2,40> & sums, int val) {
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
    return since_corner*4 + dir(dr,dc);
}

struct City {
    I2 wts;
    int nrow;
    int ncol;
    int best;
    array<I2,40> sums;

    int n1 = 4;
    int n2 = 10;

    City(const I2 & w) 
        : wts(w), nrow(w.size()), ncol(w.at(0).size()) {
        best = guess();
        sums = empty_sums();
    }

    int guess() {
        int n = 0;
        int row = 0;
        int col = 0;
        bool is_right = true;
        while(true) {
            if(is_right) {
                int nxt = col + 9 > ncol - 1 ? ncol - 1 : col + 4; 
                for(; col <= nxt; col++) {
                    //cout << row << "," << col << endl;
                    n += wts.at(row).at(col);
                }
                col--;
            } else {
                int nxt = row + 9 > nrow - 1 ? nrow - 1 : row + 4; 
                for(; row <= nxt; row++) {
                    //cout << row << "," << col << endl;
                    n += wts.at(row).at(col);
                }
                row--;
            }
            is_right = !is_right;
            
            if(row == nrow - 1 && col == ncol - 1) break;
        }
        return n;
    }

    array<I2,40> empty_sums() {
        auto i2 = I2(nrow,vector<int>(ncol,guess()+1));
        array<I2,40> sums;
        fill(sums.begin(),sums.end(),i2);
        return sums;
    }

    void best_path(int sum, int row, int col, int dr, int dc) {
        const int r0 = row + n1*dr;
        const int c0 = col + n1*dc;
        if(r0 < 0 || c0 < 0 || r0 >= nrow || c0 >= ncol) return;

        int r = row;
        int c = col;
        int s = sum;

        for(int i = 0; i < n1 - 1; i++) { // 0,1,2
            r += dr;
            c += dc;
            s += wts.at(r).at(c);
            if(s < sums.at(ix(dr,dc,i)).at(r).at(c) && sum < best) {
                sums.at(ix(dr,dc,i)).at(r).at(c) = s;
                if(r == nrow -1 && c == ncol - 1)  best = s;
            }
        }

        for(int i = n1 - 1; i < n2; i++) { // 3,4,5,6,7,8,9
            r += dr;
            c += dc;
            if(r < 0 || c < 0 || r >= nrow || c >= ncol) return;
            s += wts.at(r).at(c);
            if(s < sums.at(ix(dr,dc,i)).at(r).at(c) && sum < best) {
                sums.at(ix(dr,dc,i)).at(r).at(c) = s;
                if(r == nrow -1 && c == ncol - 1)  best = s;
                int dr1 = dc;
                int dr2 = -dc;
                int dc1 = dr;
                int dc2 = -dr;
                best_path(s,r,c,dr2,dc2);
                best_path(s,r,c,dr1,dc1);
            }
        }        
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

void part2(const string & fn) {
    auto wts = parse(fn);
    City city(wts);
    //cout << city.nrow << " x " << city.ncol << endl;
    //cout << city.nrow % 4 << endl;
    //cout << "guess : " << city.guess() << endl;

    city.best_path(0,0,0,0,1);

    int ans2 = city.guess();
    for(auto i2 : city.sums) ans2 = min(i2.back().back(),ans2);
    cout << fn << " day 17 part 1 answer = " << ans2 << endl;
}

int main() {
    //part2("test_input");
    part2("input");
    return 0;
}
