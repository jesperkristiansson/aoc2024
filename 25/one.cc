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

constexpr int WIDTH = 5;
constexpr int height = 7;
using Lock = array<uint8_t, WIDTH>;
using Key = array<uint8_t, WIDTH>;

struct LocksAndKeys{
    vector<Lock> locks;
    vector<Key> keys;
};

LocksAndKeys getLocksAndKeys(const vector<string> &lines){
    LocksAndKeys las;

    for(size_t i = 0; i < lines.size(); i += height + 1){
        bool isLock = lines[i][0] == '#';
        if(isLock){
            Lock lock;
            for(size_t j = 0; j < lines[i].size(); j++){
                uint8_t h = 0;
                while(h < height-1 && lines[i+h+1][j] == '#'){
                    h++;
                }
                lock[j] = h;
            }
            las.locks.push_back(lock);
        } else{
            Key key;
            for(size_t j = 0; j < lines[i].size(); j++){
                uint8_t h = 0;
                while(h < height-1 && lines[i + height - 2 - h][j] == '#'){
                    h++;
                }
                key[j] = h;
            }
            las.keys.push_back(key);
        }
    }

    return las;
}

void printLas(const LocksAndKeys &las){
    cout << "Locks:" << endl;
    for(const auto &l : las.locks){
        cout << "\t";
        for(const int &h : l){
            cout << h << " ";
        }
        cout << endl;
    }

    cout << "Keys:" << endl;
    for(const auto &k : las.keys){
        cout << "\t";
        for(const int &h : k){
            cout << h << " ";
        }
        cout << endl;
    }
}

bool attempt(const Lock &l, const Key &k){
    for(size_t i = 0; i < WIDTH; i++){
        if(l[i] + k[i] >= height-1){
            return false;
        }
    }
    return true;
}

int main(){
    vector<string> lines = readLines();
    LocksAndKeys las = getLocksAndKeys(lines);
    //printLas(las);

    long long cnt = 0;
    for(const Lock &l : las.locks){
        for(const Key &k : las.keys){
            if(attempt(l, k)){
                ++cnt;
            }
        }
    }

    cout << cnt << endl;
}