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

    int total = 0;
    for(size_t i = 0; i < v1.size(); i++){
        int num = v1[i];
        for(size_t j = 0; j < v2.size(); j++){
            if(v2[j] == num){
                total += num;
            }
        }
    }

    cout << total << endl;
}