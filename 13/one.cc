#include <iostream>
#include <string>
#include <sstream>
#include <climits>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <limits>
#include <utility>
#include <cctype>
#include <cstring>

#include "readInput.hpp"

using namespace std;

struct Button{
    int dx, dy;
};

struct Goal{
    int x, y;
};

struct Machine{
    Button A, B;
    Goal goal;
};

vector<Machine> readMachines(const vector<string> &lines){
    vector<Machine> machines;
    machines.reserve(lines.size()/4);

    for(size_t i = 0; i < lines.size(); i += 4){
        Machine &m = machines.emplace_back();

        sscanf(lines[i].c_str(), "Button A: X+%d, Y+%d", &m.A.dx, &m.A.dy);
        sscanf(lines[i+1].c_str(), "Button B: X+%d, Y+%d", &m.B.dx, &m.B.dy);
        sscanf(lines[i+2].c_str(), "Prize: X=%d, Y=%d", &m.goal.x, &m.goal.y);
    }

    return machines;
}

long long cost(const Machine &m){
    int denom = m.A.dx*m.B.dy - m.A.dy*m.B.dx;
    if(denom != 0){
        int numer = m.A.dx*m.goal.y - m.A.dy*m.goal.x;
        if(numer % denom != 0){   //not integer solution
            return 0;
        }
        int B = numer / denom;

        if(B < 0 || B > 100){
            return 0;
        }

        int A_numer = m.goal.x - m.B.dx*B;
        if(A_numer % m.A.dx != 0){  //not integer solution
            return 0;
        }
        int A = A_numer / m.A.dx;

        if(A < 0 || A > 100){
            return 0;
        }

        return 3*A + B;
    } else{     //A and B are just factors of each other, would need to have seperate calculation, but doesn't happen in input
        cout << "denom == 0, skipping" << endl;
        return 0;
    }
}

int main(){
    vector<string> lines = readLines();

    vector<Machine> machines = readMachines(lines);

    long long sum = 0;
    for(const Machine &m : machines){
        sum += cost(m);
    }

    cout << sum << endl;
}