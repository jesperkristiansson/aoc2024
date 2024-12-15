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

const char *word = "XMAS";
const size_t word_len = 4;

bool matchesWord(const matrix &mat, size_t row, size_t col, int dy, int dx){
    int y = row;
    int x = col;
    for(size_t i = 0; i < word_len; i++){
        if(mat[y][x] != word[i]){
            return false;
        }
        y += dy;
        x += dx;
    }

    return true;
}

int findWord(const matrix &mat, size_t row, size_t col){
    const size_t ROWS = mat.size();
    const size_t COLS = ROWS == 0 ? 0 : mat[0].size();

    vector<int> dy;
    if(row < word_len - 1){
        dy = {0, 1};
    } else if(row > ROWS - word_len){
        dy = {-1, 0};
    } else{
        dy = {-1, 0, 1};
    }

    vector<int> dx;
    if(col < word_len - 1){
        dx = {0, 1};
    } else if(col > COLS - word_len){
        dx = {-1, 0};
    } else{
        dx = {-1, 0, 1};
    }

    int found = 0;

    for(auto y : dy){
        for(auto x : dx){
            if(matchesWord(mat, row, col, y, x)){
                ++found;
            }
        }
    }

    return found;
}

int main(){
    matrix mat = readLines();

    const size_t ROWS = mat.size();
    const size_t COLS = ROWS == 0 ? 0 : mat[0].size();

    int sum = 0;

    for(size_t row = 0; row < ROWS; row++){
        for(size_t col = 0; col < COLS; col++){
            if(mat[row][col] == word[0]){
                sum += findWord(mat, row, col);
            }
        }
    }

    cout << sum << endl;
}