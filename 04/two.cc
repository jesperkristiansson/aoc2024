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

using matrix = vector<string>;

bool findWord(const matrix &mat, size_t row, size_t col){
    char upLeft = mat[row-1][col-1];
    char upRight = mat[row-1][col+1];
    char downLeft = mat[row+1][col-1];
    char downRight = mat[row+1][col+1];

    if(!((upLeft  == 'M' && downRight == 'S') || (upLeft == 'S' && downRight == 'M'))){
        return false;
    }

    if(!((downLeft  == 'M' && upRight == 'S') || (downLeft == 'S' && upRight == 'M'))){
        return false;
    }

    return true;
}

int main(){
    matrix mat = readLines();

    const size_t ROWS = mat.size();
    const size_t COLS = ROWS == 0 ? 0 : mat[0].size();

    int sum = 0;

    for(size_t row = 1; row < ROWS-1; row++){
        for(size_t col = 1; col < COLS-1; col++){
            if(mat[row][col] == 'A' && findWord(mat, row, col)){
                sum++;
            }
        }
    }

    cout << sum << endl;
}