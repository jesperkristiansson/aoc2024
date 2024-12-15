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

static constexpr const char PREFIX[] = "mul(";
static size_t PREFIX_SIZE = sizeof(PREFIX) - 1;

long doMuls(const string &line){
    size_t current_idx = 0;
    long sum = 0;
    while(true){
        size_t found_idx = line.find(PREFIX, current_idx);
        if(found_idx == string::npos){      //not found
            break;
        }

        current_idx = found_idx + PREFIX_SIZE;
        if(!isdigit(line[current_idx])){
            continue;
        }

        const char *start_p = &line[current_idx];
        char *end_p;
        long num1 = strtol(start_p, &end_p, 10);
        current_idx += end_p - start_p;

        if(line[current_idx] != ','){
            continue;
        }
        current_idx++;

        if(!isdigit(line[current_idx])){
            continue;
        }

        start_p = &line[current_idx];
        long num2 = strtol(start_p, &end_p, 10);
        current_idx += end_p - start_p;

        if(line[current_idx] != ')'){
            continue;
        }
        current_idx++;

        sum += num1*num2;
    }

    return sum;
}

int main(){
    vector<string> lines = readLines();
    long sum = 0;
    for(const auto &line : lines){
        sum += doMuls(line);
    }

    cout << sum << endl;
}