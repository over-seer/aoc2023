#include "aoc.h"

using namespace std;

auto next_dir(const vector<string> & mirrors, int row, int col, int dr, int dc) {
    array<int,4> result = {0,0,0,0};
    auto & [dr1,dc1,dr2,dc2] = result;
    int nrow = mirrors.size();
    int ncol = mirrors.at(0).size();
    //if(row < 0 || col < 0 || row >= nrow || col >= ncol) return result;
    //cout << "debug\n";
    char sym = mirrors.at(row).at(col);
    if(sym == '.' || (sym == '|' && dc == 0) || (sym == '-' && dr == 0)) { // no change
        dr1 = dr;
        dc1 = dc;
    } else if(sym == '\\') { // r->d, l->u, d->r, u->1
        dr1 = dc;
        dc1 = dr;
    } else if(sym == '/') { // r->u, l->d, d->l, u->r
        dr1 = -dc;
        dc1 = -dr;
    } else if(sym == '|' && dr == 0) { //r->u+d, l->u+d
        dr1 = 1;
        dr2 = -1;
    } else if(sym == '-' && dc == 0) { //u->l+r, d->l+r
        dc1 = 1;
        dc2 = -1;
    }
    return result;
}

void step(  set<array<int,4>> & log, 
            const vector<string> & mirrors,
            int row,
            int col,
            int dr,
            int dc) {
    int nrow = mirrors.size();
    int ncol = mirrors.at(0).size();
    if(row < 0 || col < 0 || row >= nrow || col >= ncol) return;
    //cout << row << "," << col << " " << mirrors.at(row).at(col) << " " << dr << "," << dc << " ";// << endl;
    //cin.ignore();
    if(dr == 0 && dc == 0) return;
    if(log.contains({row,col,dr,dc})) return;
    else log.insert({row,col,dr,dc});
    auto [dr1,dc1,dr2,dc2] = next_dir(mirrors,row,col,dr,dc);
    step(log,mirrors,row+dr1,col+dc1,dr1,dc1);
    //cout << "branch!" << endl;
    step(log,mirrors,row+dr2,col+dc2,dr2,dc2);
}

size_t num_energized(const vector<string> & ip, int row, int col, int dr, int dc) {
    set<array<int,4>> states;
    step(states,ip,row,col,dr,dc);
    set<array<int,2>> positions;
    for(auto [r,c,_r,_c] :states) positions.insert({r,c});
    return positions.size();
}

void part1(const string & fn) {
    size_t ans1 = num_energized(read_ip(fn),0,0,0,1);
    cout << fn << " day 16 part 1 answer = " << ans1 << endl;
}

void part2(const string & fn) {
    auto ip = read_ip(fn);
    int nrow = ip.size();
    int ncol = ip.at(0).size();

    size_t ans2 = 0;
    for(int i = 0; i < nrow; i++) {
        ans2 = max(ans2,num_energized(ip,i,0,0,1));
        ans2 = max(ans2,num_energized(ip,i,ncol - 1,0,-1));
    }
    for(int i = 0; i < ncol; i++) {
        ans2 = max(ans2,num_energized(ip,0,i,1,0));
        ans2 = max(ans2,num_energized(ip,nrow-1,i,-1,0));
    }
    cout << fn << " day 16 part 2 answer = " << ans2 << endl;

}

int main() {
    part1("test_input");
    part1("input");
    part2("test_input");
    part2("input");
    return 0;
}
