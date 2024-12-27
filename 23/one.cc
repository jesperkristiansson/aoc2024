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

struct Connection{
    string computer1, computer2;
};

struct Computer{
    string name;
    set<const Computer *> connections;
};

istream &operator>>(istream &is, Connection &c){
    getline(is, c.computer1, '-');
    getline(is, c.computer2);
    return is;
}

int main(){
    vector<Connection> connections = readInput_vec<Connection>();

    map<string, Computer> computers;
    for(const auto &connection : connections){
        auto &com1 = computers[connection.computer1];
        auto &com2 = computers[connection.computer2];

        com1.name = connection.computer1;
        com1.connections.insert(&com2);

        com2.name = connection.computer2;
        com2.connections.insert(&com1);
    }

    long score = 0;
    for(const auto &[name, comp1] : computers){
        for(const auto &comp2 : comp1.connections){
            for(const auto &comp3 : comp2->connections){
                if((comp1.name[0] == 't' || comp2->name[0] == 't' || comp3->name[0] == 't') && comp3->connections.contains(&comp1)){
                    ++score;
                }
            }
        }
    }
    score /= 6;

    cout << score << endl;

    // for(auto c : connections){
    //     cout << c.computer1 << '-' << c.computer2 << endl;
    // }
}