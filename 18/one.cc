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

int main(){
    vector<Point> points = readInput_vec<Point>();

    const int max = 71;
    Grid<char> map(max, max);
    const int toFall = 1024;
    fallDown(map, points, toFall);

    Point start(0,0);
    Point end(max-1, max-1);

    cout << findPath(map, start, end) << endl;
}