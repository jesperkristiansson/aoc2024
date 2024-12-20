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

bool inHalf(int n, int start, int size){
    return n >= start && n < (start + size);
}

long long scoreRobots(const vector<Robot> &robots, int size_x, int size_y){
    int half_size_x = size_x/2;
    int half_size_y = size_y/2;

    int halfLeftStart = 0;
    int halfRightStart = half_size_x + 1;
    int halfTopStart = 0;
    int halfBottomStart = half_size_y + 1;

    int quads[4] = {0, 0, 0, 0};
    for(const Robot &r: robots){
        if(inHalf(r.p.x, halfLeftStart, half_size_x)){
            if(inHalf(r.p.y, halfTopStart, half_size_y)){
                quads[0]++;
            } else if(inHalf(r.p.y, halfBottomStart, half_size_y)){
                quads[1]++;
            }
        } else if(inHalf(r.p.x, halfRightStart, half_size_x)){
            if(inHalf(r.p.y, halfTopStart, half_size_y)){
                quads[2]++;
            } else if(inHalf(r.p.y, halfBottomStart, half_size_y)){
                quads[3]++;
            }
        }
    }

    return (long long)quads[0] * quads[1] * quads[2] * quads[3];
}

int main(){
    vector<string> lines = readLines();

    vector<Robot> robots = readRobots(lines);

    const int size_x = 101;
    const int size_y = 103;

    moveRobots(robots, 100, size_x, size_y);

    long long sum = scoreRobots(robots, size_x, size_y);

    cout << sum << endl;
}