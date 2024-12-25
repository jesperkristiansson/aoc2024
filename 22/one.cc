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

long long advance(long long secret, int n){
    for(int i = 0; i < n; i++){
        secret = (secret ^ secret*64) % 16777216;
        secret = (secret ^ secret/32) % 16777216;
        secret = (secret ^ secret*2048) % 16777216;
    }
    return secret;
}

long long score(const vector<long long> &secrets){
    long long sum = 0;
    for(long long secret : secrets){
        sum += advance(secret, 2000);
    }
    return sum;
}

int main(){
    vector<long long> secrets = readInput_vec<long long>();
    cout << score(secrets) << endl;
}