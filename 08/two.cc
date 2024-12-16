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

struct Point{
    Point() = default;
    Point(int r, int c) : r{r}, c{c} {}
    int r,c;
    auto operator<=>(const Point &other) const = default;
};

struct Grid{
    vector<string> mat;
    int rows, cols;
};

map<char, vector<Point>> getFrequencyMap(const Grid &grid){
    map<char, vector<Point>> frequencyMap;
    for(int r = 0; r < grid.rows; r++){
        for(int c = 0; c < grid.cols; c++){
            char frequency = grid.mat[r][c];
            if(frequency != '.'){
                frequencyMap[frequency].emplace_back(r,c);
            }
        }
    }
    return frequencyMap;
}

Grid getGrid(){
    Grid grid;
    grid.mat = readLines();
    grid.rows = grid.mat.size();
    grid.cols = grid.mat[0].size();

    return grid;
}

bool isInside(Point p, const Grid &grid){
    return p.r >= 0 && p.r < grid.rows && p.c >= 0 && p.c < grid.cols;
}

set<Point> getAntinodesSingleFrequency(const Grid &grid, const vector<Point> &locations){
    set<Point> antinodes;

    for(size_t i = 0; i < locations.size(); i++){
        for(size_t j = i+1; j < locations.size(); j++){
            const auto p1 = locations[i];
            const auto p2 = locations[j];

            int dr = p2.r - p1.r;
            int dc = p2.c - p1.c;

            Point antinode = p1;
            do{
                antinodes.insert(antinode);
                antinode.r -= dr;
                antinode.c -= dc;
            } while(isInside(antinode, grid));

            antinode = p2;
            do{
                antinodes.insert(antinode);
                antinode.r += dr;
                antinode.c += dc;
            } while(isInside(antinode, grid));
        }
    }

    return antinodes;
}

set<Point> getAntinodes(const Grid &grid, const map<char, vector<Point>> &frequencyMap){
    set<Point> antinodes;

    for(const auto &[frequency, locations] : frequencyMap){
        antinodes.merge(getAntinodesSingleFrequency(grid, locations));
    }

    return antinodes;
}

int main(){
    Grid grid = getGrid();
    map<char, vector<Point>> frequencyMap = getFrequencyMap(grid);
    set<Point> antinodes = getAntinodes(grid, frequencyMap);
    cout << antinodes.size();
}