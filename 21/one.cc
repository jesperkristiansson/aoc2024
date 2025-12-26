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
#include "Grid.hpp"

using namespace std;

using Code = string;

enum Move{
    ACTIVATE,
    UP,
    RIGHT,
    DOWN,
    LEFT
};

char toChar(Move m){
    switch(m){
        case ACTIVATE:
            return 'A';
        case UP:
            return '^';
        case RIGHT:
            return '>';
        case DOWN:
            return 'v';
        case LEFT:
            return '<';
        default:
            return '*';
    }
}

void print(const vector<Move> &moves){
    for(Move m : moves){
        cout << toChar(m);
    }
    cout << endl;
}

void addMovesVerticalFirst(vector<Move> &moves, Direction d){
    Move m;

    //move up/down
    if(d.dy < 0){
        m = UP;
        d.dy = -d.dy;
    } else{
        m = DOWN;
    }
    for(int i = 0; i < d.dy; i++){
        moves.push_back(m);
    }

    //move right/left
    if(d.dx < 0){
        m = LEFT;
        d.dx = -d.dx;
    } else{
        m = RIGHT;
    }
    for(int i = 0; i < d.dx; i++){
        moves.push_back(m);
    }
}

void addMovesHorizontalFirst(vector<Move> &moves, Direction d){
    Move m;

    //move right/left
    if(d.dx < 0){
        m = LEFT;
        d.dx = -d.dx;
    } else{
        m = RIGHT;
    }
    for(int i = 0; i < d.dx; i++){
        moves.push_back(m);
    }

    //move up/down
    if(d.dy < 0){
        m = UP;
        d.dy = -d.dy;
    } else{
        m = DOWN;
    }
    for(int i = 0; i < d.dy; i++){
        moves.push_back(m);
    }
}

void addMoves(vector<Move> &moves, Point from, Point to, Point gap){
    Direction delta = to - from;

    //we avoid moving over the gap in the keypad
    if(from.x == gap.x && to.y == gap.y){
        //must move in x-direction first
        addMovesHorizontalFirst(moves, delta);
    } else if(from.y == gap.y && to.x == gap.x){
        //must move in y-direction first
        addMovesVerticalFirst(moves, delta);
    } else {
        //can move in either direction first
        addMovesHorizontalFirst(moves, delta);
        // addMovesVerticalFirst(moves, delta);
    }
}

/*------------- Keypad ------------------------*/
const map<char, Point> keypadPositions = {
    {'7', Point(0,0)},
    {'8', Point(1,0)},
    {'9', Point(2,0)},
    {'4', Point(0,1)},
    {'5', Point(1,1)},
    {'6', Point(2,1)},
    {'1', Point(0,2)},
    {'2', Point(1,2)},
    {'3', Point(2,2)},
    {'0', Point(1,3)},
    {'A', Point(2,3)}
};

const Point keypadStartPos = keypadPositions.at('A');

vector<Move> keypadMoves(const Code &code){
    vector<Move> moves;

    Point currentPos = keypadStartPos;
    for(char c : code){
        Point target = keypadPositions.at(c);
        addMoves(moves, currentPos, target, Point(0,3));
        moves.push_back(ACTIVATE);

        currentPos = target;
    }

    return moves;
}
/*---------------------------------------------*/

/*------------- Directional -------------------*/
const map<Move, Point> directionalPositions = {
    {UP, Point(1,0)},
    {ACTIVATE, Point(2,0)},
    {LEFT, Point(0,1)},
    {DOWN, Point(1,1)},
    {RIGHT, Point(2,1)}
};

const Point directionalStartPos = directionalPositions.at(ACTIVATE);

vector<Move> directionalMoves(const vector<Move> &movesIn){
    vector<Move> moves;

    Point currentPos = directionalStartPos;
    for(Move m : movesIn){
        Point target = directionalPositions.at(m);
        addMoves(moves, currentPos, target, Point(0,0));
        moves.push_back(ACTIVATE);

        currentPos = target;
    }

    return moves;
}
/*---------------------------------------------*/

int main(){
    vector<Code> codes = readLines();

    long long score = 0;
    for(const Code &code : codes){
        cout << code << endl;
        vector<Move> moves = keypadMoves(code);
        print(moves);
        for(int i = 0; i < 2; i++){
            moves = directionalMoves(moves);
            print(moves);
        }
        cout << moves.size() << " * " << stoi(code) << " = " << moves.size() * stoi(code) << endl << endl;

        score += moves.size() * stoi(code);
    }

    cout << score << endl;
}