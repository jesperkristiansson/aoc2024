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
#include <queue>

#include "readInput.hpp"
#include "Grid.hpp"

using namespace std;

void zero(Grid<char> &map){
    for(int y = 0; y < map.yMax; y++){
        for(int x = 0; x < map.xMax; x++){
            map.mat[y][x] = 0;
        }
    }
}

void fallDown(Grid<char> &map, const vector<Point> &points, int toFall){
    for(int i = 0; i < toFall; i++){
        map.at(points[i]) = '#';
    }
}

struct Node{
    Point pos;
    int cost;

    Node(Point pos, int cost) : pos{pos}, cost{cost} {}

    bool operator<(const Node &other) const{
        return this->cost > other.cost;
    }
};

int findPath(Grid<char> &maze, Point start, Point end){
    priority_queue<Node> q;
    q.emplace(start, 0);

    while(!q.empty()){
        Node n = q.top();
        q.pop();

        if(n.pos == end){
            return n.cost;
        }

        if(maze.at(n.pos) == '-'){
            continue;
        }

        maze.at(n.pos) = '-';

        for(Direction dir : {NORTH, EAST, SOUTH, WEST}){
            Point newP = n.pos + dir;
            if(maze.contains(newP) && maze.at(newP) != '#'){
                q.emplace(newP, n.cost + 1);
            }
        }
    }
    return -1;
}

void print(const Grid<char> &maze){
    for(const auto &row : maze.mat){
        for(auto c : row){
            char cp = c == 0 ? '.' : c;
            cout << cp;
        }
        cout << endl;
    }
}

int main(){
    vector<Point> points = readInput_vec<Point>();
    int maxX = 0, maxY = 0;
    for(const auto &p : points){
        if(p.x > maxX){
            maxX = p.x;
        }
        if(p.y > maxY){
            maxY = p.y;
        }
    }

    Point start(0,0);
    Point end(maxX, maxY);

    int low = 0;
    int high = points.size()-1;
    bool pathExists{};
    int mid{};
    while(low <= high){
        mid = (high + low)/2;

        Grid<char> map(maxX+1, maxY+1);
        //zero(map);
        fallDown(map, points, mid+1);
        int cost = findPath(map, start, end);
        pathExists = cost != -1;
        if(pathExists){
            low = mid+1;
        } else{
            high = mid-1;
        }
    }

    if(pathExists){
        cout << points[mid+1] << endl;
    } else{
        cout << points[mid] << endl;
    }
}