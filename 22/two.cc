#include <iostream>
#include <string>
#include <sstream>
#include <climits>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <utility>
#include <cctype>
#include <cstring>

#include "readInput.hpp"

using namespace std;

using Pattern = array<int8_t, 4>;
struct PatternHasher {
    std::size_t operator()(const Pattern& a) const {
        std::size_t h = 0;

        for (auto e : a) {
            h ^= std::hash<int8_t>{}(e)  + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};
using PatternMap = unordered_map<Pattern, int, PatternHasher>;
using PatternSet = unordered_set<Pattern, PatternHasher>;

vector<uint8_t> getPriceList(long long secret, int n){
    vector<uint8_t> priceList;
    priceList.reserve(n+1);
    priceList.push_back(secret % 10);
    for(int i = 0; i < n; i++){
        secret = (secret ^ secret << 6) & 0xFFFFFF;
        secret = (secret ^ secret >> 5) & 0xFFFFFF;
        secret = (secret ^ secret << 11) & 0xFFFFFF;
        priceList.push_back(secret % 10);
    }
    return priceList;
}

vector<int8_t> getPriceDiff(const vector<uint8_t> &prices){
    vector<int8_t> priceDiff;
    priceDiff.reserve(prices.size());

    uint8_t lastPrice = prices[0];
    for(uint8_t price : prices){
        priceDiff.push_back(price - lastPrice);
        lastPrice = price;
    }

    return priceDiff;
}

void addToPatternMap(const vector<int8_t> &priceDiff, const vector<uint8_t> &priceList, PatternMap &patternMap){
    PatternSet addedPatterns;

    for(size_t i = 3; i < priceDiff.size(); i++){
        Pattern pattern;
        pattern[0] = priceDiff[i-3];
        pattern[1] = priceDiff[i-2];
        pattern[2] = priceDiff[i-1];
        pattern[3] = priceDiff[i];

        if(!addedPatterns.contains(pattern)){
            patternMap[pattern] += priceList[i];
            addedPatterns.insert(pattern);
        }
    }
}

long long score(const vector<long long> &secrets){
    vector<vector<uint8_t>> priceLists;
    priceLists.reserve(secrets.size());

    vector<vector<int8_t>> priceDiffs;
    priceDiffs.reserve(secrets.size());
    for(long long secret : secrets){
        priceLists.push_back(getPriceList(secret, 2000));
        priceDiffs.push_back(getPriceDiff(priceLists.back()));
    }

    PatternMap patternMap;
    for(size_t i = 0; i < priceDiffs.size(); i++){
        addToPatternMap(priceDiffs[i], priceLists[i], patternMap);
    }

    auto isLess = [](const auto &e1, const auto &e2){return e1.second < e2.second;};
    auto max_it = max_element(patternMap.begin(), patternMap.end(), isLess);

    return max_it->second;
}

int main(){
    vector<long long> secrets = readInput_vec<long long>();
    cout << score(secrets) << endl;
}