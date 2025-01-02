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

using Maze = Grid<char>;

Maze toMaze(vector<string>::iterator begin, vector<string>::iterator end){
    int yMax = distance(begin, end);
    int xMax = yMax > 0 ? begin->size() : 0;
    Maze m(xMax, yMax);

    for(int r = 0; r < yMax; r++){
        for(int c = 0; c < xMax; c++){
            m.at({c, r}) = (*begin)[c];
        }
        begin++;
    }

    return m;
}

Point findStart(const Maze &maze){
    for(int y = 0; y < maze.yMax; y++){
        for(int x = 0; x < maze.xMax; x++){
            if(maze.at({x,y}) == 'S'){
                return {x,y};
            }
        }
    }
    cerr << "did not find start position" << endl;
    return {-1,-1};
}

Point findEnd(const Maze &maze){
    for(int y = 0; y < maze.yMax; y++){
        for(int x = 0; x < maze.xMax; x++){
            if(maze.at({x,y}) == 'E'){
                return {x,y};
            }
        }
    }
    cerr << "did not find start position" << endl;
    return {-1,-1};
}

Grid<int> findPath(const Maze &maze){
    Grid<int> stepsLeft(maze.xMax, maze.yMax);

    Point end = findEnd(maze);

    Point current = end;
    Direction dir = NORTH;
    while(maze.at(current + dir) == '#'){
        dir = dir.rotateRight();
    }

    int dist = 0;
    while(maze.at(current) != 'S'){
        if(maze.at(current + dir) != '#'){
            current += dir;
        } else if(maze.at(current + dir.rotateRight()) != '#'){
            current += dir.rotateRight();
            dir = dir.rotateRight();
        } else{
            current += dir.rotateLeft();
            dir = dir.rotateLeft();
        }

        stepsLeft.at(current) = ++dist;
    }

    return stepsLeft;
}

vector<Direction> getDirections(int range){
    vector<Direction> dirs;
    dirs.reserve((range+1)*(range+1));

    for(int x = -range; x <= range; x++){
        int xAbs = abs(x);
        int yRange = range - xAbs;
        for(int y = -yRange; y <= yRange; y++){
            dirs.emplace_back(x, y);
        }
    }

    return dirs;
}

constexpr int cheatLimit = 100;
constexpr int cheatRange = 20;

long long findCheats(const Maze &maze, const Grid<int> &stepsLeft){
    Point start = findStart(maze);

    vector<Direction> dirs = getDirections(cheatRange);

    Point current = start;
    Direction dir = NORTH;
    while(maze.at(current + dir) == '#'){
        dir = dir.rotateRight();
    }

    long long count = 0;
    while(maze.at(current) != 'E'){
        for(Direction d : dirs){
            Point p = current + d;
            if(maze.contains(p) && maze.at(p) != '#'){
                int stepsSaved = stepsLeft.at(current) - stepsLeft.at(p) - (abs(d.dx) + abs(d.dy));
                if(stepsSaved >= cheatLimit){
                    ++count;
                }
            }
        }

        Direction nextDir = dir;
        if(maze.at(current + dir) != '#'){
            nextDir = dir;
        } else if(maze.at(current + dir.rotateRight()) != '#'){
            nextDir = dir.rotateRight();
        } else{
            nextDir = dir.rotateLeft();
        }

        dir = nextDir;
        current += dir;
    }

    return count;
}

int main(){
    vector<string> lines = readLines();
    Maze maze = toMaze(lines.begin(), lines.end());
    Grid<int> stepsLeft = findPath(maze);
    cout << findCheats(maze, stepsLeft) << endl;
}