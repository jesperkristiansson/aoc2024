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


void addMovesVertical(vector<Move> &moves, Direction d){
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
}

void addMovesHorizontal(vector<Move> &moves, Direction d){
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
}


void addMovesVerticalFirst(vector<Move> &moves, Direction d){
    addMovesVertical(moves, d);
    addMovesHorizontal(moves, d);
}

void addMovesHorizontalFirst(vector<Move> &moves, Direction d){
    addMovesHorizontal(moves, d);
    addMovesVertical(moves, d);
}

vector<vector<Move>> getPossibleMoves(Point from, Point to, Point gap){
    Direction delta = to - from;

    vector<vector<Move>> possibleMoves;

    //we avoid moving over the gap in the keypad
    if(from.x == gap.x && to.y == gap.y){
        //must move in x-direction first
        possibleMoves.emplace_back();
        addMovesHorizontalFirst(possibleMoves.back(), delta);
    } else if(from.y == gap.y && to.x == gap.x){
        //must move in y-direction first
        possibleMoves.emplace_back();
        addMovesVerticalFirst(possibleMoves.back(), delta);
    } else {
        //can move in either direction first
        possibleMoves.emplace_back();
        addMovesHorizontalFirst(possibleMoves.back(), delta);
        if(!(from.x == to.x || from.y == to.y)){
            //if doesnt give same result
            possibleMoves.emplace_back();
            addMovesVerticalFirst(possibleMoves.back(), delta);
        }
    }

    return possibleMoves;
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

vector<vector<Move>> keypadMoves_brute(char from, char to){
    Point current = keypadPositions.at(from);
    Point target = keypadPositions.at(to);
    vector<vector<Move>> moves = getPossibleMoves(current, target, Point(0,3));
    for(auto &m : moves){
        m.push_back(ACTIVATE);
    }
    return moves;
}

void simplify_helper(vector<vector<Move>> &moves, size_t i1, size_t n, const vector<vector<vector<Move>>> &moves2, size_t i2){
    if(i2 == moves2.size()){
        return;
    }

    const auto &ms2 = moves2[i2];

    n /= ms2.size();

    for(size_t i2_1 = 0; i2_1 < ms2.size(); i2_1++){
        const auto &m2 = ms2[i2_1];
        for(size_t i1_1 = 0; i1_1 < n; i1_1++){
            auto &m1 = moves[i1_1+i1];
            m1.reserve(m1.size() + m2.size());
            m1.insert(m1.end(), m2.begin(), m2.end());
        }

        simplify_helper(moves, i1, n, moves2, i2+1);
        i1 += n;
    }
}

void simplify(vector<vector<Move>> &moves, const vector<vector<vector<Move>>> &moves2){
    size_t numCombinations = 1;
    for(const auto &m : moves2){
        numCombinations *= m.size();
    }

    size_t start_i = moves.size();
    size_t end_i = start_i + numCombinations;
    moves.resize(end_i);
    simplify_helper(moves, start_i, numCombinations, moves2, 0);
}

vector<vector<Move>> directionalMoves_brute(const vector<vector<Move>> &movesIn){
    vector<vector<Move>> moves;

    for(const vector<Move> &possibleMoves : movesIn){
        Point currentPos = directionalStartPos;

        vector<vector<vector<Move>>> moves2;
        for(Move m : possibleMoves){
            Point target = directionalPositions.at(m);
            moves2.push_back(getPossibleMoves(currentPos, target, Point(0,0)));
            for(auto &m : moves2.back()){
                m.push_back(ACTIVATE);
            }
            currentPos = target;
        }

        simplify(moves, moves2);
    }

    return moves;
}

vector<vector<Move>> bruteForce(const vector<Code> codes){
    vector<vector<Move>> allMoves;
    for(const Code &code : codes){
        vector<Move> moves;
        auto keypadPos = 'A';
        for(char c : code){
            vector<vector<Move>> possibleMoves = keypadMoves_brute(keypadPos, c);
            for(int i = 0; i < 2; i++){
                possibleMoves = directionalMoves_brute(possibleMoves);
            }

            size_t minI = 0;
            for(size_t i = 1; i < possibleMoves.size(); i++){
                if(possibleMoves[i].size() < possibleMoves[minI].size()){
                    minI = i;
                }
            }

            const auto &minMoves = possibleMoves[minI];
            moves.reserve(moves.size() + minMoves.size());
            moves.insert(moves.end(), minMoves.begin(), minMoves.end());

            keypadPos = c;
        }

        allMoves.push_back(moves);
    }

    return allMoves;
}

int main(){
    vector<Code> codes = readLines();

    vector<vector<Move>> allMoves = bruteForce(codes);
    long long score = 0;
    for(size_t i = 0; i < codes.size(); i++){
        const auto &code = codes[i];
        const auto &moves = allMoves[i];
        score += moves.size() * stoi(code);
    }

    cout << score << endl;
}