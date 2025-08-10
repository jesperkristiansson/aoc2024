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

using Matrix = Grid<char>;
using BoolMatrix = Grid<unsigned char>;

struct Score{
    long area{0};
    long perimeter{0};
};

Score findRegion(const Matrix &mat, BoolMatrix &visited, char c, Point p){
    visited.at(p) = true;

    Score score;
    score.area++;

    vector<Direction> directions = {NORTH, EAST, SOUTH, WEST};
    for(Direction d : directions){
        Point newP = p + d;
        if(!mat.contains(newP) || mat.at(newP) != c){
            score.perimeter++;
            continue;
        }

        if(visited.at(newP)){
            continue;
        }

        Score s = findRegion(mat, visited, c, newP);
        score.area += s.area;
        score.perimeter += s.perimeter;
    }

    return score;
}

int main(){
    auto lines = readLines();
    Matrix mat(lines);
    BoolMatrix visited(mat.xMax, mat.yMax);

    long long scoreSum = 0;
    for(int y = 0; y < mat.yMax; y++){
        for(int x = 0; x < mat.xMax; x++){
            Point p(x, y);
            if(visited.at(p)){
                continue;
            }

            Score score = findRegion(mat, visited, mat.at(p), p);
            scoreSum += score.area * score.perimeter;
        }
    }

    cout << scoreSum << endl;
}