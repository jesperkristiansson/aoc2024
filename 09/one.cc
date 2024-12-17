#include <iostream>
#include <iomanip>
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

using digit = uint8_t;

digit toDigit(char c){
    return c - '0';
}

int main(){
    string line;
    getline(cin, line);

    vector<digit> diskMap;
    diskMap.reserve(line.size());

    size_t totalLength = 0;
    for(char c : line){
        digit digit = toDigit(c);
        diskMap.push_back(digit);
        totalLength += digit;
    }

    vector<int> disk;
    disk.reserve(totalLength);

    bool isFile = true;
    int fileId = 0;
    for(digit d : diskMap){
        int id = -1;
        if(isFile){
            id = fileId++;
        }
        isFile = !isFile;
        for(int i = 0; i < d; i++){
            disk.push_back(id);
        }
    }

    size_t left_idx = 0;
    size_t right_idx = totalLength-1;
    while(left_idx < right_idx){
        while(disk[left_idx] != -1){
            ++left_idx;
        }
        while(disk[right_idx] == -1){
            --right_idx;
        }

        if(left_idx >= right_idx){
            break;
        }

        disk[left_idx] = disk[right_idx];
        disk[right_idx] = -1;
        left_idx++;
        right_idx--;
    }

    long long sum = 0;
    long idx = 0;
    while(disk[idx] != -1){
        sum += disk[idx]*idx;
        ++idx;
    }

    cout << "sum: " << sum << endl;
}