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
#include <tuple>

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

string getPassword(vector<const Computer *> clique){
    string password;
    std::sort(clique.begin(), clique.end(), [](const Computer *comp1, const Computer *comp2){return comp1->name < comp2->name;});
    for(auto *comp : clique){
        password += comp->name;
        password += ',';
    }
    password.pop_back();
    return password;
}

vector<const Computer *> findClique(vector<const Computer *> computers, size_t size){
    if(computers.size() < size){
        return {};
    }
    if(computers.size() == size){
        for(size_t i = 0; i < computers.size(); i++){
            const Computer *computerToCheck = computers[i];
            for(size_t j = 0; j < computers.size(); j++){
                if(i == j){
                    continue;
                }
                if(!computerToCheck->connections.contains(computers[j])){
                    return {};
                }
            }
        }
        return computers;
    }

    // computers.size() > size
    vector<const Computer *> newComputers = computers;
    newComputers.pop_back();
    for(int i = (int) computers.size()-1; i >= 0; i--){
        auto res = findClique(newComputers, size);
        if(!res.empty()){
            return res;
        }

        if(i > 0){
            newComputers[i-1] = computers[i];
        }
    }

    return {};
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

    vector<const Computer *> maxClique;
    for(auto &kv : computers){
        auto &comp = kv.second;

        //+1 needed?
        vector<vector<const Computer*>> ncn(comp.connections.size()+1);
        for(const auto *neigh : comp.connections){
            int numCommonNeighbours = 1;
            for(const auto *neigh2 : neigh->connections){
                if(comp.connections.contains(neigh2)){
                    numCommonNeighbours++;
                }
            }
            ncn[numCommonNeighbours].push_back(neigh);
        }

        size_t cnt = 0;
        std::vector<const Computer*> availableComputers;
        for(int i = ncn.size()-1; i >= 0; i--){
            if(i + 1 <= (int)maxClique.size()){
                break;
            }

            auto &newComputers = ncn[i];
            cnt += newComputers.size();
            std::copy(newComputers.begin(), newComputers.end(), std::back_inserter(availableComputers));
            if((int)cnt < i){
                continue;
            }

            //check clique
            auto res = findClique(availableComputers, i);
            if(!res.empty()){
                res.push_back(&comp);
                maxClique = res;
                break;
            }
        }
    }

    cout << getPassword(maxClique) << endl;
}
