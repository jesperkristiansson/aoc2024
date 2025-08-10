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

Point getStartPoint(const Grid<char> &grid){
    for(int y = 0; y < grid.yMax; y++){
        for(int x = 0; x < grid.xMax; x++){
            Point p(x, y);
            if(grid.at(p) == '^'){
                return p;
            }
        }
    }
    return Point(-1, -1);
}

bool checkLoop(const Grid<char> &grid, Point pos, Direction dir){
    std::map<Point, set<Direction>> steps;
    while(grid.contains(pos)){
        if(steps[pos].contains(dir)){
            return true;
        }
        steps[pos].insert(dir);
        Point next = pos + dir;
        if(!grid.contains(next)){
            break;
        }

        if(grid.at(next) == '#'){     //turn right
            dir = dir.rotateRight();
        } else{
            pos = next;
        }
    }

    return false;
}

int main(){
    vector<string> mat = readLines();
    Grid<char> grid(mat);
    Grid<unsigned char> tried(grid.xMax, grid.yMax);

    Point pos = getStartPoint(grid);
    Direction dir = NORTH;

    int cnt = 0;
    while(grid.contains(pos)){
        Point next = pos + dir;
        if(!grid.contains(next)){
            break;
        }

        if(grid.at(next) == '#'){     //turn right
            dir = dir.rotateRight();
        } else{
            if(!tried.at(next)){
                tried.at(next) = true;
                grid.at(next) = '#';
                cnt += checkLoop(grid, pos, dir);
                grid.at(next) = '.';
            }

            pos = next;
        }
    }

    cout << cnt << endl;
}