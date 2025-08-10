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

void findRegion(const Matrix &mat, BoolMatrix &visited, char c, Point p, Score &score, map<Direction, vector<Point>> &edges){
    visited.at(p) = true;

    score.area++;

    vector<Direction> directions = {NORTH, EAST, SOUTH, WEST};
    for(Direction d : directions){
        Point newP = p + d;
        if(!mat.contains(newP) || mat.at(newP) != c){
            edges[d].push_back(newP);
            continue;
        }

        if(visited.at(newP)){
            continue;
        }

        findRegion(mat, visited, c, newP, score, edges);
    }
}

long getVertSides(vector<Point> &points){
    sort(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
    });
    long sides = 0;
    for(auto it = points.begin(); it != points.end();){
        sides++;
        Point p = *it;
        it++;
        while(it != points.end() && *it == p + Direction({0, 1})){
            p = *it;
            it++;
        }
    }
    return sides;
}

long getHoriSides(vector<Point> &points){
    sort(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x);
    });
    long sides = 0;
    for(auto it = points.begin(); it != points.end();){
        sides++;
        Point p = *it;
        it++;
        while(it != points.end() && *it == p + Direction({1, 0})){
            p = *it;
            it++;
        }
    }
    return sides;
}


Score findRegion(const Matrix &mat, BoolMatrix &visited, Point p){
    Score score;
    map<Direction, vector<Point>> edges;
    findRegion(mat, visited, mat.at(p), p, score, edges);

    score.perimeter = getVertSides(edges[WEST]) + getVertSides(edges[EAST]) + getHoriSides(edges[NORTH]) + getHoriSides(edges[SOUTH]);
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

            Score score = findRegion(mat, visited, p);
            scoreSum += score.area * score.perimeter;
        }
    }

    cout << scoreSum << endl;
}