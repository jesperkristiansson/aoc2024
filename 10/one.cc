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

size_t getScore(const Grid<char> &grid, Point p, char prev, Grid<unsigned char> &visited){
    if(!grid.contains(p) || visited.at(p) || grid.at(p) != prev + 1){
        return 0;
    }
    visited.at(p) = true;

    if(grid.at(p) == '9'){
        return 1;
    }

    return getScore(grid, p + NORTH, grid.at(p), visited)
        + getScore(grid, p + EAST, grid.at(p), visited)
        + getScore(grid, p + SOUTH, grid.at(p), visited)
        + getScore(grid, p + WEST, grid.at(p), visited)
        ;
}

size_t getScore(const Grid<char> &grid, Point p){
    Grid<unsigned char> visited(grid.xMax, grid.yMax);
    return getScore(grid, p, '0' - 1, visited);
}

size_t getScore(const Grid<char> &grid){
    size_t score = 0;
    for(int y = 0; y < grid.yMax; y++){
        for(int x = 0; x < grid.xMax; x++){
            Point p{x, y};
            if(grid.at(p) == '0'){
                score += getScore(grid, p);
            }
        }
    }
    return score;
}


int main(){
    auto mat = readLines();

    Grid<char> grid(mat);

    auto score = getScore(grid);

    cout << score << endl;
}