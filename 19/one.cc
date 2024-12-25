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

vector<string> getAvailablePatterns(const string &line){
    vector<string> availablePatterns;

    istringstream ss(line);
    while(!ss.eof()){
        string token;
        ss >> token;

        if(token.back() == ','){
            token.pop_back();
        }

        availablePatterns.push_back(token);
    }

    return availablePatterns;
}

vector<string> getTowels(vector<string>::const_iterator begin, vector<string>::const_iterator end){
    vector<string> towels;

    while(begin != end){
        towels.push_back(*begin);
        ++begin;
    }

    return towels;
}

bool isSubstrOf(const string &substr, const string &s, size_t s_i){
    size_t sizeLeft = s.size() - s_i;
    if(substr.size() > sizeLeft){
        return false;
    }

    for(size_t i = 0; i < substr.size(); i++){
        if(substr[i] != s[s_i + i]){
            return false;
        }
    }
    return true;
}

bool isPossible(const vector<string> &patterns, const string &towel, size_t towel_i){
    if(towel_i >= towel.size()){
        return true;
    }

    for(const string &pattern : patterns){
        if(!isSubstrOf(pattern, towel, towel_i)){
            continue;
        }

        if(isPossible(patterns, towel, towel_i + pattern.size())){
            return true;
        }
    }
    return false;
}

int countPossible(const vector<string> &patterns, const vector<string> &towels){
    int possible = 0;
    for(const string &towel : towels){
        if(isPossible(patterns, towel, 0)){
            ++possible;
        }
    }

    return possible;
}

int main(){
    vector<string> lines = readLines();
    vector<string> availablePatterns = getAvailablePatterns(lines[0]);
    vector<string> towels = getTowels(lines.begin() + 2, lines.end());

    cout << countPossible(availablePatterns, towels) << endl;
}