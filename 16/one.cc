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

using Maze = Grid<char>;

Maze toMaze(vector<string>::iterator begin, vector<string>::iterator end){
    int yMax = distance(begin, end);
    int xMax = yMax > 0 ? begin->size() : 0;
    Maze m(xMax, yMax);

    for(int r = 0; r < yMax; r++){
        for(int c = 0; c < xMax; c++){
            m.at({c, r}) = (*begin)[c];
        }
        begin++;
    }

    return m;
}

Point findStart(const Maze &maze){
    for(int y = 0; y < maze.yMax; y++){
        for(int x = 0; x < maze.xMax; x++){
            if(maze.at({x,y}) == 'S'){
                return {x,y};
            }
        }
    }
    cerr << "did not find start position" << endl;
    return {-1,-1};
}

struct Node{
    Point pos;
    Direction dir;
    int cost;

    Node(Point pos, Direction dir, int cost) : pos{pos}, dir{dir}, cost{cost} {}

    bool operator<(const Node &other) const{
        return this->cost > other.cost;
    }
};

int findPath(const Maze &maze, Grid<char> &visited, Point start){
    priority_queue<Node> q;
    q.emplace(start, EAST, 0);

    while(!q.empty()){
        Node n = q.top();
        q.pop();

        if(visited.at(n.pos)){
            continue;
        }

        visited.at(n.pos) = true;

        if(maze.at(n.pos) == 'E'){
            return n.cost;
        }

        Direction forward = n.dir;
        Direction right = n.dir.rotateRight();
        Direction left = n.dir.rotateLeft();

        Point forwardPos = n.pos + forward;
        Point rightPos = n.pos + right;
        Point leftPos = n.pos + left;

        if(maze.at(forwardPos) != '#'){
            q.emplace(forwardPos, forward, n.cost + 1);
        }
        if(maze.at(rightPos) != '#'){
            q.emplace(rightPos, right, n.cost + 1001);
        }
        if(maze.at(leftPos) != '#'){
            q.emplace(leftPos, left, n.cost + 1001);
        }
    }
    return -1;
}

int main(){
    vector<string> lines = readLines();
    Maze maze = toMaze(lines.begin(), lines.end());
    Point start = findStart(maze);
    Grid<char> visited(maze.xMax, maze.yMax);

    long long sum = findPath(maze, visited, start);

    cout << sum << endl;
}