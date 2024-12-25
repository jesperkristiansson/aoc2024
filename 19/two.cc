#include <iostream>
#include <string>
#include <sstream>
#include <climits>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
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

long long countPossible2(const vector<string> &patterns, const string &towel, size_t towel_i, unordered_map<size_t, long long> &cache){
    if(towel_i >= towel.size()){
        return 1;
    }

    auto it = cache.find(towel_i);
    if(it != cache.end()){
        return it->second;
    }

    long long count = 0;
    for(const string &pattern : patterns){
        if(!isSubstrOf(pattern, towel, towel_i)){
            continue;
        }

        count += countPossible2(patterns, towel, towel_i + pattern.size(), cache);
    }

    cache[towel_i] = count;
    return count;
}

long long countPossible(const vector<string> &patterns, const vector<string> &towels){
    long long possible = 0;
    for(const string &towel : towels){
        unordered_map<size_t, long long> cache;
        possible += countPossible2(patterns, towel, 0, cache);
    }

    return possible;
}

int main(){
    vector<string> lines = readLines();
    vector<string> availablePatterns = getAvailablePatterns(lines[0]);
    vector<string> towels = getTowels(lines.begin() + 2, lines.end());

    cout << countPossible(availablePatterns, towels) << endl;
}