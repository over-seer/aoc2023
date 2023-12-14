#include "aoc.h"

using namespace std;

void roll_north(vector<string> & rocks) {
    int nr = rocks.size();
    int nc = rocks.at(0).size();
    for(int c = 0; c < nc; c++) {
        for(int r = 0; r < nr; r++) {  
            if(rocks.at(r).at(c) == 'O') {
                //found a round rock - let's roll
                //careful we're looping down to -1
                for(int r1 = r - 1; r1 >= -1; r1--) {
                    if(r1 == -1 || rocks.at(r1).at(c) != '.') {
                        rocks.at(r).at(c) = '.';
                        rocks.at(r1+1).at(c) = 'O';
                        break;
                    }
                }
            }
        }
    }
}

void roll_west(vector<string> & rocks) {
    int nr = rocks.size();
    int nc = rocks.at(0).size();
    for(int r = 0; r < nr; r++) {
        for(int c = 0; c < nc; c++) {  
            if(rocks.at(r).at(c) == 'O') {
                //found a round rock - let's roll
                bool is_stopped = false;
                //careful we're looping down to -1
                for(int c1 = c - 1; c1 >= -1; c1--) {
                    if(c1 == -1 || rocks.at(r).at(c1) != '.') {
                        rocks.at(r).at(c) = '.';
                        rocks.at(r).at(c1 + 1) = 'O';
                        break;
                    }
                }
            }
        }
    }
}

void roll_south(vector<string> & rocks) {
    int nr = rocks.size();
    int nc = rocks.at(0).size();
    for(int c = 0; c < nc; c++) {
        for(int r = nr - 1; r >= 0; r--) {  
            if(rocks.at(r).at(c) == 'O') {
                //found a round rock - let's roll
                //careful we're looping to number of rows
                for(int r1 = r + 1; r1 <= nr; r1++) {
                    if(r1 == nr || rocks.at(r1).at(c) != '.') {
                        rocks.at(r).at(c) = '.';
                        rocks.at(r1-1).at(c) = 'O';
                        break;
                    }
                }
            }
        }
    }
}

void roll_east(vector<string> & rocks) {
    int nr = rocks.size();
    int nc = rocks.at(0).size();
    for(int r = 0; r < nr; r++) {
        for(int c = nc - 1; c >= 0; c--) {  
            if(rocks.at(r).at(c) == 'O') {
                //found a round rock - let's roll
                //careful we're looping to number of cols
                for(int c1 = c + 1; c1 <= nc; c1++) {
                    if(c1 == nc || rocks.at(r).at(c1) != '.') {
                        rocks.at(r).at(c) = '.';
                        rocks.at(r).at(c1 - 1) = 'O';
                        break;
                    }
                }
            }
        }
    }
}

int total_load(const vector<string> & rocks) {
    int nr = rocks.size();
    int nc = rocks.at(0).size();
    int sum = 0;
    for(int c = 0; c < nc; c++) {
        for(int r = 0; r < nr; r++) {   
            if(rocks.at(r).at(c) == 'O') {
                sum += nr - r;
            }
        }
    }
    return sum;
}

void part1(const string & fn) {
    auto ip = read_ip(fn);
    //for(string s : ip) cout << s << endl;
    roll_north(ip); 
    int ans1 = total_load(ip);
    //for(string s : ip) cout << s << endl;
    cout << fn << " day 14 part 1 answer = " << ans1 << endl;

}

void cycle(vector<string> & rocks) {
    roll_north(rocks);
    roll_west(rocks);
    roll_south(rocks);
    roll_east(rocks);
}

void part2(const string & fn) {
    auto ip = read_ip(fn);
    map<vector<string>,int> log = {{ip,0}};
    map<int,int> load_map;
    int period, i0;
    int i = 0;
    //start cycling
    while(true) { 
        i++;
        cycle(ip);

        if(auto it = log.find(ip); it != log.end()) {
            //if already seen this pattern of rocks, record starting point
            //of repetitive cycle and period and break out of loop
            i0 = it->second;
            period = i - i0;
            break;
        } else {
            //otherwise record the load for this iteration and add this
            //pattern to the log
            log[ip] = i;
            load_map[i] = total_load(ip);
        }
    }
    //for(auto [i,j] : load_map) cout << i << ":" << j << endl;
    //calculate which interation big number corresponds
    int target = (1000000000 - i0) % period + i0;
    int ans2 = load_map.at(target);
    cout << fn << " day 14 part 2 answer = " << ans2 << endl;

}

int main() {
    //part1("test_input");
    part1("input");
    //part2("test_input");
    part2("input");
    return 0;
}
