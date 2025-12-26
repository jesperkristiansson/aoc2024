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
#include <cassert>

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

struct dpKey{
    Point current;
    Point target;

    auto operator<=>(const dpKey &other) const = default;
};

using DpMap = std::map<dpKey, long long>;

// DpMap for directional keypad
DpMap getInitialDpMap(){
    DpMap dpMap;

    for(const auto &[key1, value1] : directionalPositions){
        for(const auto &[key2, value2] : directionalPositions){
            long long cost = (long long) (value1 != value2);
            cost = 1;
            dpMap[{.current = value1, .target = value2}] = cost;
        }
    }

    return dpMap;
}

template<typename K, typename V>
V getMapValue(const map<K,V> &map, const K &key){
    auto it = map.find(key);
    assert(it != map.end());
    return it->second;
}

DpMap updateDirectionalDpMap(const DpMap &dpMapIn){
    DpMap dpMap;

    for(const auto &[key1, value1] : directionalPositions){
        for(const auto &[key2, value2] : directionalPositions){
            if(value1 == Point(2,0) && value2 == Point(0,1)){
                static int cnt = 0;
                cnt += 0;
            }
            if(value1 == value2){
                dpMap[{.current = value1, .target = value2}] = 1;
            } else{
                auto allPossibleMoves = getPossibleMoves(value1, value2, Point(0, 0));
                long long minCost = numeric_limits<long long>::max();
                for(const auto &possibleMoves : allPossibleMoves){
                    auto currentPos = directionalStartPos;
                    long long cost = 0;
                    for(Move m : possibleMoves){
                        auto target = directionalPositions.at(m);
                        cost += getMapValue(dpMapIn, {.current = currentPos, .target = target});
                        currentPos = target;
                    }
                    cost += getMapValue(dpMapIn, {.current = currentPos, .target = directionalPositions.at(ACTIVATE)});

                    if(cost < minCost){
                        minCost = cost;
                    }
                }
                dpMap[{.current = value1, .target = value2}] = minCost;
            }
        }
    }

    return dpMap;
}

DpMap updateKeypadDpMap(const DpMap &dpMapIn){
    DpMap dpMap;

    for(const auto &[key1, value1] : keypadPositions){
        for(const auto &[key2, value2] : keypadPositions){
            if(value1 == value2){
                dpMap[{.current = value1, .target = value2}] = 1;
            } else{
                auto allPossibleMoves = getPossibleMoves(value1, value2, Point(0, 3));
                long long minCost = numeric_limits<long long>::max();
                for(const auto &possibleMoves : allPossibleMoves){
                    auto currentPos = directionalStartPos;
                    long long cost = 0;
                    for(Move m : possibleMoves){
                        auto target = directionalPositions.at(m);
                        cost += getMapValue(dpMapIn, {.current = currentPos, .target = target});
                        currentPos = target;
                    }
                    cost += getMapValue(dpMapIn, {.current = currentPos, .target = directionalPositions.at(ACTIVATE)});

                    if(cost < minCost){
                        minCost = cost;
                    }
                }
                dpMap[{.current = value1, .target = value2}] = minCost;
            }
        }
    }

    return dpMap;
}

int main(){
    vector<vector<vector<Move>>> dp;
    vector<Code> codes = readLines();

    DpMap dpMap = getInitialDpMap();

    for(int i = 0; i < 25; i++){
        dpMap = updateDirectionalDpMap(dpMap);
    }
    dpMap = updateKeypadDpMap(dpMap);

    long long score = 0;
    for(size_t i = 0; i < codes.size(); i++){
        const auto &code = codes[i];
        auto currentPos = keypadStartPos;
        long long cost = 0;
        for(char c : code){
            auto target = keypadPositions.at(c);
            cost += dpMap[{.current = currentPos, .target = target}];
            currentPos = target;
        }
        score += cost * stoi(code);
    }

    cout << score << endl;
}