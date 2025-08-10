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

int main(){
    vector<string> mat = readLines();
    Grid<char> grid(mat);

    Point pos = getStartPoint(grid);
    Direction dir = NORTH;

    int cnt = 0;
    while(grid.contains(pos)){
        if(grid.at(pos) != 'X'){
            ++cnt;
            grid.at(pos) = 'X';
        }
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

    cout << cnt << endl;
}