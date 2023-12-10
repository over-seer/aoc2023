//WARNING the pipe type symbol for the starting position is
//hard-coded in the main function - this might be different
//for different inputs but can be trivially determined from 
//inspection of the input 
#include "aoc.h"

using namespace std;

//check if neighbouring space is valid - i.e. within bounds and with a matching pipe type
bool ok(vector<string> & flr, int lastrow, int lastcol, int drow, int dcol) {
    int nextrow = lastrow + drow;
    int nextcol = lastcol + dcol;
    
    char c = flr[nextrow][nextcol];
    if(nextrow < 0 || nextcol < 0 || nextrow >= flr.size() || nextcol >= flr[0].size()) 
        return false;

    //this bit not needed except for starting position?
    if(drow == 1) { 
        return c == 'J' || c == 'L' || c=='|';
    } else if(drow == -1) {
        return c == '7' || c == 'F' || c=='|';
    } else if(dcol == 1) {
        return c == 'J' || c == '7' || c=='-';
    } else if(dcol == -1) {
        return c == 'L' || c == 'F' || c=='-';
    }
    return false;
}

// returns which two directions are possible based on pipe type
// or all four directions if on starting position
vector<array<int,2>> which_dir(char c) {
    if(c == 'J') return {{-1,0},{0,-1}};
    else if(c == '7') return {{1,0},{0,-1}};
    else if(c == 'F') return {{1,0},{0,1}};
    else if(c == 'L') return {{-1,0},{0,1}};
    else if(c == '-') return {{0,-1},{0,1}};
    else if(c == '|') return {{-1,0},{1,0}};
    //try all ways at the start
    else if(c == 'S'){
        return {
            array<int,2>{ 0, 1},
            array<int,2>{ 0,-1},
            array<int,2>{ 1, 0},
            array<int,2>{-1, 0}
        };
    } else throw(std::runtime_error("error"));
}


// returns next position based on last position's pipe type and already-visited positions
optional<array<int,2>> next(vector<string> & flr, int lastrow, int lastcol) {
    char c = flr[lastrow][lastcol];
    for(auto [drow,dcol] : which_dir(c)) {
        int row = lastrow + drow;
        int col = lastcol + dcol;
        if(ok(flr,lastrow,lastcol,drow,dcol)) {
            return {{row,col}};
        }
    }
    return nullopt;
}

//locate the starting 'S' position
array<int,2> start(vector<string> & ip) {
    int r0,c0;
    int nrow = ip.size();
    int ncol = ip[0].size();
    //cout << nrow << "x" << ncol << endl;
    for(int row = 0; row < nrow; row++) {
    for(int col = 0; col < ncol; col++) {
        char c = ip[row][col];
        if(c == 'S') {
            return {row,col};
        }
    }
    }
    //unless bad input should never reach here
    throw runtime_error("bad input no starting point");
}

//returns a set of all positions on path
set<array<int,2>> find_path(const string & fn) {
    auto ip = read_ip(fn);
    int ans1 = 0;
    auto [r0,c0] = start(ip);
    set<array<int,2>> path;

    while(true) {
        path.insert({r0,c0});
        ans1++;
        array<int,2> rc;
        if(auto rc = next(ip,r0,c0)) {
            auto [r,c] = *rc;
            ip[r0][c0] = 'S';
            r0 = r;
            c0 = c;
        } else {
            break;
        }
    }
    return path;
}


void part1(const string & fn) {
    auto path = find_path(fn);
    //furthest point is half the path length
    cout << fn << " day 10 part 1 answer = " << path.size()/2 << endl; 
}

void part2(const string & fn, char replace_s) {
    auto ip = read_ip(fn);

    //get path of the loop so we can remove all other 'junk' pipes
    auto path = find_path(fn);

    //get starting positions and grid sizes
    auto [r,c] = start(ip);
    int nr = (int)ip.size();
    int nc = (int)ip[0].size();

    //remove junk pipes
    for(int i = 0; i < nr; i++) {
        for(int j = 0; j < nc; j++) {
            if(!path.contains({i,j})) {
                ip[i][j] = '.';
            }
        }
    }

    //replace starting symbol 'S' with actual pipe type
    ip[r][c] = replace_s;


    size_t ans = 0;
    for(string line : ip) {
        bool is_out = true;
        bool was_eff_not_ell;
        for(char c : line) {
            if(c == '.') {
                if(!is_out) {
                    ans++;
                }
            } else if(c == '|') {
                is_out = !is_out;
            } else if(c == 'F') {
                was_eff_not_ell = true;
            } else if(c == 'L') {
                was_eff_not_ell = false;
            } else if(c == 'J') {
                if(was_eff_not_ell) {
                    is_out = !is_out;
                }
            } else if(c == '7') {
                if(!was_eff_not_ell) {
                    is_out = !is_out;
                }
            } else if(c == '-') {
                //do nothing
            } else {
                cout << c <<  endl;
                throw runtime_error("bad symbol!");
            }
        }
    }
    cout << fn << " day 10 part 2 answer = " << ans << endl;
    
    exit(0);
    
}

int main() {
    //part1("test_input");
    part1("input");
    //part2("test_input2",'F');
    //part2("test_input3",'7');
    part2("input",'7');
    return 0;
}
