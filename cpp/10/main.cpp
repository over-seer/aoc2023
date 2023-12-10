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

//converts pipe type into 3x3 "wall" representation
array<array<bool,3>,3> pipe_type_walls(char c) {
   const array<array<bool,3>,3> none = {
                array<bool,3>{false,false,false},
                array<bool,3>{false,false,false},
                array<bool,3>{false,false,false}
   };

   const array<array<bool,3>,3> acr = {
        array<bool,3>{false,false,false},
        array<bool,3>{ true, true, true},
        array<bool,3>{false,false,false}
   };

   const array<array<bool,3>,3> upp = {
        array<bool,3>{false, true,false},
        array<bool,3>{false, true,false},
        array<bool,3>{false, true,false}
   };

   const array<array<bool,3>,3> jay = {
        array<bool,3>{false, true,false},
        array<bool,3>{ true, true,false},
        array<bool,3>{false,false,false}
   };

   const array<array<bool,3>,3> ell = {
        array<bool,3>{false, true,false},
        array<bool,3>{false, true, true},
        array<bool,3>{false,false,false}
   };

   const array<array<bool,3>,3> eff = {
        array<bool,3>{false,false,false},
        array<bool,3>{false, true, true},
        array<bool,3>{false, true,false}
   };

   const array<array<bool,3>,3> sev = {
        array<bool,3>{false,false,false},
        array<bool,3>{ true, true,false},
        array<bool,3>{false, true,false}
   };

    switch(c) {
        case '.':
            return none;
        case '-':
            return acr;
        case '|':
            return upp;
        case 'J':
            return jay;
        case 'L':
            return ell;
        case 'F':
            return eff;
        case '7':
            return sev;
        default:
            throw runtime_error("bad pipe!");
    }
}
    
//converts mxn grid of pipe types to 3mx3n grid with with pipes becoming "walls"          
auto detailed_pipes(const vector<string> & ip) {
    vector<vector<bool>> pipes(ip.size()*3,vector<bool>(ip[0].size()*3,false));
    for(size_t row = 0; row < ip.size(); row++) {
        string s = ip[row];
        for(size_t col = 0; col < s.size(); col++) {
            char c = ip[row][col];
            auto pipe = pipe_type_walls(c);
            for(size_t i : {0,1,2}) {
                for(size_t j : {0,1,2}) {
                    size_t ii = row*3;
                    size_t jj = col*3;
                    pipes[ii+i][jj+j] = pipe[i][j];
                }
            }
        }
    }
    return pipes;
}


//from starting point r0,c0, recursive search for unvisited locations
//recursion terminates when neighbouring locations are already visited
//(or pipes, which are represented as visited locations)
void search(vector<vector<bool>> & visited, int r0, int c0) {
    array<array<int,2>,4> dirs = {
            array<int,2>{ 0, 1},
            array<int,2>{ 0,-1},
            array<int,2>{ 1, 0},
            array<int,2>{-1, 0}
    };

    int nr = (int)visited.size();
    int nc = (int)visited[0].size();

    if(visited[r0][c0]) return;
    else visited[r0][c0] = true;

    for(auto [dr,dc] : dirs) {
        int r = r0 + dr;
        int c = c0 + dc;
        if(r >= 0 && c >= 0 && r < nr  && c < nc) {
            search(visited,r,c);
        }
    }
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

    //get the detailed 'wall' representation of the pipes
    auto detailed = detailed_pipes(ip);

    //fill in all accessible detailed locations outside the loop.
    //now we've removed all junk pipes, everywhere outside loop
    //should be reachable from top corner
    search(detailed,0,0);

    //loop over original coarser input grid and count as trapped if ALL
    //nine detailed locations corresponding to the location have not been
    //visited
    size_t ans2 = 0;
    for(int r = 0; r < nr; r++) {
        for(int c = 0; c < nc; c++) {
            bool is_trapped = true;
            for(int ii : {0,1,2}) {
                for(int jj : {0,1,2}) {
                    if(detailed[r*3+ii][c*3+jj]) {
                        is_trapped = false;
                        break;
                    }
                }
                if(!is_trapped) break;
            }
            if(is_trapped) {
                ip[r][c] = 'I';
                ans2++;
            } else {
                ip[r][c] = '.';

            }
        }
    }

    for(auto line : ip) {
        //cout << line << endl;
    }

    cout << fn << " day 10 part 2 answer = " << ans2 << endl; 
    
}

int main() {
    //part1("test_input");
    part1("input");
    //part2("test_input2",'F');
    //part2("test_input3",'7');
    part2("input",'7');
    return 0;
}
