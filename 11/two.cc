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

using Stone = unsigned long long;

static Stone from_string(const string &s){
    return stoull(s);
}

map<pair<Stone, int>, long long> stoneAmountMap;

long long getNumStones(Stone s, int blinks){
    if(blinks == 0){
        return 1;
    }

    long long &count = stoneAmountMap[make_pair(s, blinks)];
    if(count != 0){     //value exists;
        return count;
    }

    if(s == 0){
        count = getNumStones(1, blinks-1);
    } else if(string stoneString = to_string(s); stoneString.size() % 2 == 0){
        size_t halfSize = stoneString.size()/2;
        Stone left = from_string(stoneString.substr(0, halfSize));
        Stone right = from_string(stoneString.substr(halfSize));
        count = getNumStones(left, blinks-1) + getNumStones(right, blinks-1);
    } else{
        count = getNumStones(s*2024, blinks-1);
    }

    return count;
}

int main(){
    vector<Stone> stones = readInput_vec<Stone>();

    long long sumStones = 0;
    for(Stone stone : stones){
        sumStones += getNumStones(stone, 75);
    }

    cout << sumStones << endl;
}