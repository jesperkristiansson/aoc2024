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

struct Position{
    int r,c;
};

struct Direction{
    int dr, dc;
};

bool isInside(Position pos, size_t ROWS, size_t COLS){
    return pos.r >= 0 && pos.r < (int)ROWS && pos.c >= 0 && pos.c < (int)COLS;
}

Position nextPos(Position current, Direction dir){
    Position next;
    next.r = current.r + dir.dr;
    next.c = current.c + dir.dc;
    return next;
}

Direction turnRight(Direction dir){
    Direction newDir;
    if(dir.dr == 0 && dir.dc == 1){     //right
        newDir.dr = 1;
        newDir.dc = 0;
    } else if(dir.dr == 0 && dir.dc == -1){     //left
        newDir.dr = -1;
        newDir.dc = 0;
    } else if(dir.dr == 1 && dir.dc == 0){     //down
        newDir.dr = 0;
        newDir.dc = -1;
    } else if(dir.dr == -1 && dir.dc == 0){     //up
        newDir.dr = 0;
        newDir.dc = 1;
    } else{
        //error
        newDir.dr = 0;
        newDir.dc = 0;
        cout << "Error: unknown direction" << endl;
    }

    return newDir;
}

char &symbolAt(matrix &mat, Position pos){
    return mat[pos.r][pos.c];
}

Position getStartPosition(const matrix &mat, size_t ROWS, size_t COLS){
    for(size_t r = 0; r < ROWS; r++){
        for(size_t c = 0; c < COLS; c++){
            if(mat[r][c] == '^'){
                Position pos;
                pos.r = (int)r;
                pos.c = (int)c;
                return pos;
            }
        }
    }
}

Direction getStartDirection(){
    Direction dir;
    dir.dr = -1;
    dir.dc = 0;
    return dir;
}

int main(){
    matrix mat = readLines();

    const size_t ROWS = mat.size();
    const size_t COLS = mat[0].size();

    cout << ROWS << " " << COLS << endl;

    Position pos = getStartPosition(mat, ROWS, COLS);
    Direction dir = getStartDirection();

    int cnt = 0;
    while(isInside(pos, ROWS, COLS)){
        if(symbolAt(mat, pos) != 'X'){
            ++cnt;
            symbolAt(mat, pos) = 'X';
        }
        Position next = nextPos(pos, dir);
        if(!isInside(next, ROWS, COLS)){
            break;
        }

        if(symbolAt(mat, next) == '#'){     //turn right
            dir = turnRight(dir);
        } else{
            pos = next;
        }

    }

    cout << cnt << endl;
}