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
#include <iterator>

#include "readInput.hpp"
#include "Grid.hpp"

using namespace std;

using Warehouse = Grid<char>;

Warehouse toWarehouse(vector<string>::iterator begin, vector<string>::iterator end){
    int yMax = distance(begin, end);
    int xMax = yMax > 0 ? begin->size() : 0;
    Warehouse wh(xMax, yMax);

    for(int r = 0; r < yMax; r++){
        for(int c = 0; c < xMax; c++){
            wh.at({c, r}) = (*begin)[c];
        }
        begin++;
    }

    return wh;
}

Direction toDirection(char c){
    switch(c){
        case '^':
            return Direction(0, -1);
        case 'v':
            return Direction(0, 1);
        case '>':
            return Direction(1, 0);
        case '<':
            return Direction(-1, 0);
        default:
            cerr << "unknown move" << endl;
            return Direction(0, 0);
    }
}

vector<Direction> getMoves(vector<string>::iterator begin, vector<string>::iterator end){
    vector<Direction> moves;
    moves.reserve(distance(begin, end)*begin->size());

    while(begin != end){
        for(char c : *begin){
            moves.push_back(toDirection(c));
        }

        begin++;
    }

    return moves;
}

Point findRobot(const Warehouse &wh){
    for(int y = 0; y < wh.yMax; y++){
        for(int x = 0; x < wh.xMax; x++){
            if(wh.at({x,y}) == '@'){
                return {x,y};
            }
        }
    }
    return {-1,-1};
}

void performMoves(Warehouse &wh, const vector<Direction> &moves){
    Point robot = findRobot(wh);

    for(const Direction &move : moves){
        Point nextPos = robot + move;

        char symbol = wh.at(nextPos);
        if(symbol == '.'){
            wh.at(robot) = '.';
            wh.at(nextPos) = '@';

            robot = nextPos;
        } else if(symbol == '#'){
            continue;
        } else if(symbol == 'O'){
            Point end = nextPos;
            while(wh.at(end) == 'O'){
                end += move;
            }

            symbol = wh.at(end);
            if(symbol == '.'){
                wh.at(robot) = '.';
                wh.at(nextPos) = '@';
                wh.at(end) = 'O';

                robot = nextPos;
            } else if(symbol == '#'){
                continue;
            } else{
                cerr << "unrecognized symbol " << symbol << endl;
                continue;
            }
        } else{
            cerr << "unrecognized symbol " << symbol << endl;
            continue;
        }
    }
}

long long score(const Warehouse &wh){
    long long score = 0;
    for(int y = 0; y < wh.yMax; y++){
        for(int x = 0; x < wh.xMax; x++){
            if(wh.at({x, y}) == 'O'){
                score += 100*y + x;
            }
        }
    }
    return score;
}

int main(){
    vector<string> lines = readLines();
    auto isEmpty = [](const string &s){return s.empty();};

    auto mapEnd = find_if(lines.begin(), lines.end(), isEmpty);

    Warehouse wh = toWarehouse(lines.begin(), mapEnd);
    vector<Direction> moves = getMoves(mapEnd+1, lines.end());

    performMoves(wh, moves);

    long long sum = score(wh);

    cout << sum << endl;
}