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

static void printStones(const vector<Stone> &stones){
    return;
    for(Stone s : stones){
        cout << s << " ";
    }
    cout << endl;
}

int main(){
    vector<Stone> stones = readInput_vec<Stone>();

    printStones(stones);
    cout << endl;

    int sumStones = 0;
    for(Stone stone : stones){
        vector<Stone> currentStones = {stone};
        for(size_t i = 0; i < 25; i++){
            size_t numStones = currentStones.size();    // we will add to the vector but only want to go through the original stones
            for(size_t j = 0; j < numStones; j++){
                Stone &s = currentStones[j];
                //rules
                if(s == 0){
                    s = 1;
                } else if(string stoneString = to_string(s); stoneString.size() % 2 == 0){
                    size_t halfSize = stoneString.size()/2;
                    Stone left = from_string(stoneString.substr(0, halfSize));
                    Stone right = from_string(stoneString.substr(halfSize));
                    s = left;
                    currentStones.push_back(right);
                } else{
                    s *= 2024;
                }
            }
            printStones(currentStones);
        }
        cout << endl;
        sumStones += currentStones.size();
    }

    cout << sumStones << endl;
}