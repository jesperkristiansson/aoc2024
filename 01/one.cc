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

using namespace std;

int main(){
    vector<int> v1, v2;
    while(!cin.eof()){
        int left, right;
        cin >> left;
        cin >> right;
        v1.push_back(left);
        v2.push_back(right);
    }

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    int dist = 0;
    for(size_t i = 0; i < v1.size(); i++){
        dist += std::abs(v2[i] - v1[i]);
    }

    cout << dist << endl;
}