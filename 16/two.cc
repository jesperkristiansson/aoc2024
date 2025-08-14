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

struct Pose{
    Point pos;
    Direction dir;

    auto operator<=>(const Pose &other) const = default;
};

std::ostream &operator<<(std::ostream &os, const Pose &p){
    return os << p.pos << '/' << p.dir;
}

struct Node{
    Pose pose;
    int cost;
    Pose prev;

    Node(Point pos, Direction dir, int cost, Pose prev) : pose{.pos{pos}, .dir{dir}}, cost{cost}, prev{prev} {}

    bool operator<(const Node &other) const{
        return this->cost > other.cost;
    }
};

int getNumTiles(map<Pose, vector<Pose>> &prev, Grid<unsigned char> &visited, set<Pose> &visitedPoses, Pose pos){
    if(visitedPoses.contains(pos)){
        return 0;
    }
    visitedPoses.insert(pos);
    int numTiles = visited.at(pos.pos) ? 0 : 1;
    visited.at(pos.pos) = true;
    for(Pose p : prev[pos]){
        numTiles += getNumTiles(prev, visited, visitedPoses, p);
    }
    return numTiles;
}

int findPath(const Maze &maze, Point start){
    priority_queue<Node> q;
    q.emplace(start, EAST, 0, Pose{});

    map<Pose, vector<Pose>> prev;
    map<Pose, int> prevCosts;

    while(!q.empty()){
        Node n = q.top();
        Pose p = n.pose;
        Pose prevP = n.prev;
        int cost = n.cost;
        q.pop();

        if(prev.contains(p)){
            if(cost == prevCosts[p]){
                prev[p].push_back(prevP);
            }
            continue;
        }

        prev[p].push_back(prevP);
        prevCosts[p] = cost;

        if(maze.at(p.pos) == 'E'){
            Grid<unsigned char> visited(maze.xMax, maze.yMax);
            set<Pose> visitedPoses;
            int numTiles = getNumTiles(prev, visited, visitedPoses, p);
            while(!q.empty() && maze.at(q.top().pose.pos) == 'E' && q.top().cost == cost){
                prev[q.top().pose].push_back(q.top().prev);
                numTiles += getNumTiles(prev, visited, visitedPoses, q.top().pose);
                q.pop();
            }
            // minus 1 because the (0,0) is also counted
            return numTiles - 1;
        }

        Direction forward = p.dir;
        Direction right = p.dir.rotateRight();
        Direction left = p.dir.rotateLeft();

        Point forwardPos = p.pos + forward;
        Point rightPos = p.pos + right;
        Point leftPos = p.pos + left;

        if(maze.at(forwardPos) != '#'){
            q.emplace(forwardPos, forward, cost + 1, p);
        }
        if(maze.at(rightPos) != '#'){
            q.emplace(rightPos, right, cost + 1001, p);
        }
        if(maze.at(leftPos) != '#'){
            q.emplace(leftPos, left, cost + 1001, p);
        }
    }
   return -1;
}

int main(){
    vector<string> lines = readLines();
    Maze maze = toMaze(lines.begin(), lines.end());
    Point start = findStart(maze);

    long long sum = findPath(maze, start);

    cout << sum << endl;
}