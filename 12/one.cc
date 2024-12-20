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

using Matrix = vector<string>;
using BoolMatrix = vector<vector<bool>>;

BoolMatrix makeBoolMatrix(int rows, int cols){
    BoolMatrix bm;
    bm.reserve(rows);
    for(int i = 0; i < rows; i++){
        bm.emplace_back(cols);
    }

    return bm;
}

struct Score{
    long area{0};
    long perimeter{0};
};

struct Point{
    Point() = default;
    Point(int r, int c) : r{r}, c{c} {}
    int r,c;
    auto operator<=>(const Point &other) const = default;
};

struct Direction{
    int dr, dc;
};

bool isInside(Point p, int rows, int cols){
    return p.r >= 0 && p.r < rows && p.c >= 0 && p.c < cols;
}

Score findRegion(const Matrix &mat, BoolMatrix &visited, int rows, int cols, char c, Point p){
    visited[p.r][p.c] = true;

    Score score;
    score.area++;

    vector<Direction> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    for(Direction d : directions){
        Point newP(p.r + d.dr, p.c + d.dc);
        if(!isInside(newP, rows, cols) || mat[newP.r][newP.c] != c){
            score.perimeter++;
            continue;
        }

        if(visited[newP.r][newP.c]){
            continue;
        }

        Score s = findRegion(mat, visited, rows, cols, c, newP);
        score.area += s.area;
        score.perimeter += s.perimeter;
    }

    return score;
}

int main(){
    Matrix mat = readLines();
    int rows = mat.size();
    int cols = mat[0].size();

    BoolMatrix visited = makeBoolMatrix(rows, cols);

    long long scoreSum = 0;
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            if(visited[r][c]){
                continue;
            }

            Score score = findRegion(mat, visited, rows, cols, mat[r][c], Point(r, c));
            scoreSum += score.area * score.perimeter;
        }
    }

    cout << scoreSum << endl;
}