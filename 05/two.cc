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

int main(){
    vector<string> lines = readLines();

    set<pair<int, int>> rules;
    auto it = lines.begin();
    while(*it != ""){
        const auto &line = *it;
        istringstream ss(line);
        int before, after;
        ss >> before;
        ss.ignore();
        ss >> after;

        rules.emplace(before, after);
        ++it;
    }

    ++it;

    vector<vector<int>> updates;
    while(it != lines.end()){
        const auto &line = *it;

        updates.emplace_back();
        istringstream ss(line);
        while(ss){
            int num;
            ss >> num;
            updates.back().push_back(num);
            ss.ignore();
        }

        ++it;
    }

    int sum = 0;
    for(const auto &update : updates){
        bool good = true;
        for(size_t i = 0; i < update.size() && good; i++){
            int first = update[i];
            for(size_t j = i+1; j < update.size(); j++){
                int second = update[j];
                pair<int,int> p(first,second);
                if(rules.find(p) == rules.end()){
                    good = false;
                    break;
                }
            }
        }
        if(!good){
            vector<int> ordered;
            ordered.reserve(update.size());

            for(auto n : update){
                auto it = ordered.begin();
                for(; it != ordered.end(); it++){
                    pair<int,int> p(n, *it);
                    if(rules.find(p) != rules.end()){
                        ordered.insert(it, n);
                        break;
                    }
                }
                if(it == ordered.end()){
                    ordered.push_back(n);
                }
            }

            sum += ordered[ordered.size()/2];
        }
    }

    cout << sum << endl;
}