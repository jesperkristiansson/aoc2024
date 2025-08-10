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
    Warehouse wh(xMax*2, yMax);

    for(int r = 0; r < yMax; r++){
        for(int c = 0; c < xMax; c++){
            char ch = (*begin)[c];
            switch(ch){
                case '#':
                    wh.at({c*2, r}) = '#';
                    wh.at({c*2 + 1 , r}) = '#';
                    break;
                case '.':
                    wh.at({c*2, r}) = '.';
                    wh.at({c*2 + 1 , r}) = '.';
                    break;
                case 'O':
                    wh.at({c*2, r}) = '[';
                    wh.at({c*2 + 1 , r}) = ']';
                    break;
                case '@':
                    wh.at({c*2, r}) = '@';
                    wh.at({c*2 + 1 , r}) = '.';
                    break;
                default:
                    cerr << "Unrecognized symbol: " << ch << endl;
                    break;
            }
        }
        begin++;
    }

    return wh;
}

Direction toDirection(char c){
    switch(c){
        case '^':
            return NORTH;
        case 'v':
            return SOUTH;
        case '>':
            return EAST;
        case '<':
            return WEST;
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

void performBoxMovement(Warehouse &wh, Direction dir, const vector<Point> &boxLocations){
    for(Point p : boxLocations){
        wh.at(p) = '.';
        wh.at(p + EAST) = '.';
    }
    for(Point p : boxLocations){
        wh.at(p + dir) = '[';
        wh.at(p + EAST + dir) = ']';
    }
}

bool boxMovementCheckVert(Warehouse &wh, Point pos, Direction dir, vector<Point> &boxLocations){
    Point posLeft;
    Point posRight;
    if(wh.at(pos) == '['){
        boxLocations.push_back(pos);
        posLeft = pos + dir;
        posRight = pos + dir + EAST;
    } else{
        boxLocations.push_back(pos + WEST);
        posLeft = pos + dir + WEST;
        posRight = pos + dir;
    }

    if(wh.at(posLeft) == '#' || wh.at(posRight) == '#'){
        return false;
    }

    if(wh.at(posLeft) == '['){
        return boxMovementCheckVert(wh, posLeft, dir, boxLocations);
    }

    if(wh.at(posLeft) == ']' && !boxMovementCheckVert(wh, posLeft, dir, boxLocations)){
        return false;
    }
    if(wh.at(posRight) == '[' && !boxMovementCheckVert(wh, posRight, dir, boxLocations)){
        return false;
    }

    return true;
}

bool boxMovementCheckHori(Warehouse &wh, Point pos, Direction dir, vector<Point> &boxLocations){
    Point end = pos;
    while(wh.at(end) == '[' || wh.at(end) == ']'){
        Direction boxLocationCorrection = wh.at(end) == '[' ? Direction(0, 0) : Direction(-1, 0);
        boxLocations.push_back(end + boxLocationCorrection);
        end += dir*2;
    }

    return wh.at(end) == '.';
}

bool moveBox(Warehouse &wh, Point pos, Direction dir){
    vector<Point> boxLocations;
    bool canMove;
    if(dir == NORTH || dir == SOUTH){
        canMove = boxMovementCheckVert(wh, pos, dir, boxLocations);
    } else{
        canMove = boxMovementCheckHori(wh, pos, dir, boxLocations);
    }

    if(canMove){
        performBoxMovement(wh, dir, boxLocations);
    }
    return canMove;
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
        } else if(symbol == '[' || symbol == ']'){
            if(moveBox(wh, nextPos, move)){
                wh.at(robot) = '.';
                wh.at(nextPos) = '@';

                robot = nextPos;
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
            if(wh.at({x, y}) == '['){
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