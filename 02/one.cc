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

bool isSafe(vector<int> level){
    bool increasing = level[1] > level[0];
    auto cond = increasing ? [](int val){ return val >= 1 && val <= 3;} : [](int val){ return val <= -1 && val >= -3;};
    for(size_t i = 1; i < level.size(); i++){
        if(!cond(level[i]-level[i-1])){
            return false;
        }
    }
    return true;
}

int main(){
    vector<vector<int>> levels = readInput_mat<int>();

    int num_safe = 0;
    for(const auto &level : levels){
        if(isSafe(level)){
            num_safe++;
        }
    }

    cout << num_safe << endl;
}