#include "aoc.h"

using namespace std;

string read_num(const string & line, int i) {
    string snum = "";
    while(i < line.size() && isdigit(line.at(i))) {
        snum += line.at(i);
        i++;
    }
    return snum;
}

bool is_symbol(const vector<string> & scheme, int i, int j) {
    int ni = scheme.size();
    int nj = scheme.at(0).size();
    return i>0 && j>0 && i<ni && j<nj && scheme.at(i).at(j) != '.';
}

vector<array<int,2>> neighbours(int i, int j, int len) {
    vector<array<int,2>> nbrs;
    nbrs.push_back({i,j-1});
    nbrs.push_back({i,j+len});
    for(int ii : {i-1,i+1}) {
        for(int jj = j-1; jj<=j+len; jj++) {
            nbrs.push_back({ii,jj});
        }
    }
    return nbrs;
}

bool is_symbol_neighbour(const vector<string> & scheme, int i, int j, int len) {
    for(auto [ii,jj] : neighbours(i,j,len)) {
        if(is_symbol(scheme,ii,jj)) return true;
    }
    return false;
}

void gear_check(const vector<string> & scheme,
                int i,
                int j,
                const string & sn,
                map<array<int,2>,vector<string>> & gear_map) {
    for(auto [ii,jj] : neighbours(i,j,sn.size())) {
        if(is_symbol(scheme,ii,jj) && scheme.at(ii).at(jj) == '*') {
            gear_map[{ii,jj}].push_back(sn);
        }
    }
}

void day3(const string & fn) {
    const auto ip = read_ip(fn);
    map<array<int,2>,vector<string>> gears_map;
    int ni = ip.size();
    int nj = ip.at(0).size();
    int ans = 0;
    for(int i = 0; i < ni; i++) {
        int j = 0;
        while(j < nj) {
            string line = ip[i];
            string sn = read_num(line,j);
            if(sn.empty()) {
                j++;
            } else {
                if(is_symbol_neighbour(ip,i,j,sn.size())) {
                    ans += stoi(sn);
                }
                gear_check(ip,i,j,sn,gears_map);
                j += sn.size();
            }
        }
    }
    cout << fn << " day 3 part 1 answer = " << ans << endl;
    int ans2 = 0;
    for(auto [ij,sns] : gears_map) {
        if(sns.size() > 1) {
            ans2 += stoi(sns.at(0)) * stoi(sns.at(1));
        }
    }
    cout << fn << " day 3 part 2 answer = " << ans2 << endl;
}

int main() {
    //day3("test_input");
    day3("input");
    return 0;
}
