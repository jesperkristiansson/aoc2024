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

struct Equation{
    long target;
    vector<long> operands;
};

Equation getEquation(const string &line){
    Equation eq;

    stringstream ss(line);
    ss >> eq.target;
    ss.ignore();
    while(ss){
        long num;
        ss >> num;

        if(!ss){
            break;
        }

        eq.operands.push_back(num);
    }

    return eq;
}

vector<Equation> getEquations(const vector<string> &lines){
    vector<Equation> eqs;
    for(const auto &line : lines){
        eqs.push_back(getEquation(line));
    }
    return eqs;
}

bool helper(const Equation &eq, size_t idx, long ack){
    if(idx >= eq.operands.size()){
        return ack == eq.target;
    }

    long sum_add = ack + eq.operands[idx];
    if(helper(eq, idx+1, sum_add)){
        return true;
    }
    long sum_mul = ack * eq.operands[idx];
    if(helper(eq, idx+1, sum_mul)){
        return true;
    }

    return false;
}

bool verifyEquation(const Equation &eq){
    return helper(eq, 1, eq.operands[0]);
}

long sumEquations(const vector<Equation> &eqs){
    long sum = 0;
    for(const auto &eq : eqs){
        if(verifyEquation(eq)){
            sum += eq.target;
        }
    }
    return sum;
}

int main(){
    vector<string> lines = readLines();
    vector<Equation> eqs = getEquations(lines);
    cout << sumEquations(eqs);
}