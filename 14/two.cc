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

struct Point{
    int x,y;
    auto operator<=>(const Point &other) const = default;
};

struct Direction{
    int dx, dy;
};

struct Robot{
    Point p;
    Direction d;
};

vector<Robot> readRobots(const vector<string> &lines){
    vector<Robot> robots;
    robots.reserve(lines.size());

    for(const string &line : lines){
        Robot &r = robots.emplace_back();
        sscanf(line.c_str(), "p=%d,%d v=%d,%d", &r.p.x, &r.p.y, &r.d.dx, &r.d.dy);
    }

    return robots;
}

int newPos(int p, int d, int s, int max){
    int newP = ((long long)p + (long long)d*s) % max;
    if(newP < 0){
        newP += max;
    }
    return newP;
}

void moveRobots(vector<Robot> &robots, int seconds, int size_x, int size_y){
    for(Robot &r : robots){
        r.p.x = newPos(r.p.x, r.d.dx, seconds, size_x);
        r.p.y = newPos(r.p.y, r.d.dy, seconds, size_y);
    }
}

void printRobots(const vector<Robot> &robots, int size_x, int size_y){
    set<Point> robotPositions;
    for(const Robot &r : robots){
        robotPositions.insert(r.p);
    }

    for(int y = 0; y < size_y; y++){
        for(int x = 0; x < size_x; x++){
            Point p;
            p.x = x;
            p.y = y;
            cout << (robotPositions.contains(p) ? 'X' : '.');
        }
        cout << '\n';
    }
    cout << endl;
}

int main(){
    vector<string> lines = readLines();

    vector<Robot> robots = readRobots(lines);

    const int size_x = 101;
    const int size_y = 103;

    for(int i = 0; i < 10000; i++){
        cout << i << endl;
        printRobots(robots, size_x, size_y);

        moveRobots(robots, 1, size_x, size_y);
    }
}