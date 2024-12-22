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

uint8_t singlePass(uint64_t A){
    int B = ((A & 0b111) ^ 0b101);
    return (B ^ ((A >> B) & 0b111)) ^ 0b110;
}

int main(){
    vector<uint8_t> expectedOutput = {2,4,1,5,7,5,1,6,0,3,4,6,5,5,3,0};
    vector<uint64_t> possiblePrefixes = {0b000};
    for(int i = (int)expectedOutput.size() - 1; i >= 0; i--){
        uint8_t o = expectedOutput[i];

        vector<uint64_t> nextPrefixes;
        for(uint64_t prefix : possiblePrefixes){
            for(uint8_t oct = 0b000; oct <= 0b111; oct++){
                uint64_t num = (prefix << 3) | oct;
                if(singlePass(num) == o){
                    nextPrefixes.push_back(num);
                }
            }
        }

        possiblePrefixes = nextPrefixes;
    }

    cout << *min_element(possiblePrefixes.begin(), possiblePrefixes.end()) << endl;
}