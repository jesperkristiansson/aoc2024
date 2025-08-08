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
#include <cassert>

#include "readInput.hpp"

using namespace std;

struct Connection{
    string computer1, computer2;
};

struct Computer{
    string name;
    set<const Computer *> connections;
    bool visited{false};
};

istream &operator>>(istream &is, Connection &c){
    getline(is, c.computer1, '-');
    getline(is, c.computer2);
    return is;
}

bool isConnectedToClique(const Computer *computer, const vector<Computer *> &clique){
    for(const Computer *computer2: clique){
        if(!computer->connections.contains(computer2)){
            return false;
        }
    }
    return true;
}

bool getCliqueHelper(const vector<Computer *> &computers, size_t size, vector<Computer *> &current){
    if(computers.size() == size){
        return true;
    }

    for(Computer *computer : computers){
        if(computer->visited || !isConnectedToClique(computer, current)){
            continue;
        }
        current.push_back(computer);
        computer->visited = true;
        if(getCliqueHelper(computers, size, current)){
            return true;
        }
        computer->visited = false;
        current.pop_back();
    }
    return false;
}


vector<Computer *> getClique(const vector<Computer *> &computers, size_t size, bool &success){
    for(auto *computer : computers){
        computer->visited = false;
    }
    vector<Computer *> clique;
    success = getCliqueHelper(computers, size, clique);
    return clique;
}

string getPassword(vector<Computer *> clique){
    string password;
    std::sort(clique.begin(), clique.end(), [](const Computer *comp1, const Computer *comp2){return comp1->name > comp2->name;});
    for(auto *comp : clique){
        password += comp->name;
    }
    return password;
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

    std::vector<std::vector<Computer *>> connectionSets(computers.size());
    for(auto &[name, computer] : computers){
        connectionSets[computer.connections.size()].push_back(&computer);
    }
    size_t cnt = 0;
    std::vector<Computer *> availableComputers;
    for(int i = connectionSets.size() - 1; i >= 0; i--){
        auto &newComputers = connectionSets[i];
        cnt += newComputers.size();
        std::copy(newComputers.begin(), newComputers.end(), std::back_inserter(availableComputers));
        if(cnt > connectionSets[i].size()){
            bool success;
            cout << "finding clique of size " << i << " from " << availableComputers.size() << " nodes" << endl;
            auto clique = getClique(availableComputers, i, success);
            //assert((clique.size() == (size_t)i) == success);
            if(success){
                cout << "Success" << endl;
                string password = getPassword(clique);
                cout << password << endl;
                break;
            }
        }
    }

    long score = 0;
    // for(const auto &[name, comp1] : computers){
    //     for(const auto &comp2 : comp1.connections){
    //         for(const auto &comp3 : comp2->connections){
    //             if((comp1.name[0] == 't' || comp2->name[0] == 't' || comp3->name[0] == 't') && comp3->connections.contains(&comp1)){
    //                 ++score;
    //             }
    //         }
    //     }
    // }
    // score /= 6;

    cout << score << endl;

    // for(auto c : connections){
    //     cout << c.computer1 << '-' << c.computer2 << endl;
    // }
}
