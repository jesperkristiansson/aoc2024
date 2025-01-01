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

enum WireState{
    UNKNOWN,
    OFF,
    ON
};

using Wire = string;
using WireStates = map<Wire, WireState>;

WireState getState(const WireStates &states, const Wire &wire, WireState defaultState = UNKNOWN){
    auto it = states.find(wire);
    if(it == states.end()){
        return defaultState;
    }

    return it->second;
}

enum Operator{
    AND,
    OR,
    XOR
};

struct Gate{
    Wire input1, input2;
    Wire output;
    Operator op;
};

struct State{
    WireStates wires;
    vector<Gate> gates;
};

WireStates getWires(vector<string>::const_iterator begin, vector<string>::const_iterator end){
    WireStates ws;
    while(begin != end){
        string name = begin->substr(0, 3);
        WireState state = (*begin)[5] == '1' ? ON : OFF;
        ws.emplace(name, state);

        begin++;
    }
    return ws;
}

Operator toOperator(const string &s){
    switch(s[0]){
        case 'A': return AND;
        case 'O': return OR;
        default: return XOR;
    }
}

vector<Gate> getGates(vector<string>::const_iterator begin, vector<string>::const_iterator end){
    vector<Gate> gates;
    gates.reserve(distance(begin, end));
    while(begin != end){
        Gate g;

        g.input1 = begin->substr(0, 3);
        auto secondSpacePos = begin->find(' ', 4);
        g.op = toOperator(begin->substr(4, secondSpacePos - 4));
        g.input2 = begin->substr(secondSpacePos+1, 3);
        g.output = begin->substr(begin->size() - 3);
        gates.push_back(g);

        begin++;
    }
    return gates;
}

State getStartingState(const vector<string> &lines){
    State s;

    auto it = find(lines.begin(), lines.end(), "");
    s.wires = getWires(lines.begin(), it);
    s.gates = getGates(it + 1, lines.end());

    return s;
}

WireState performOperation(Operator op, WireState i1, WireState i2){
    switch(op){
        case AND:
            if(i1 == OFF || i2 == OFF){
                return OFF;
            } else if(i1 == UNKNOWN || i2 == UNKNOWN){
                return UNKNOWN;
            } else{
                return ON;
            }
        case OR:
            if(i1 == ON || i2 == ON){
                return ON;
            } else if(i1 == UNKNOWN || i2 == UNKNOWN){
                return UNKNOWN;
            } else{
                return OFF;
            }
        default:    //XOR
            if(i1 == OFF && i2 == ON){
                return ON;
            } else if(i1 == ON && i2 == OFF){
                return ON;
            } else if(i1 == UNKNOWN || i2 == UNKNOWN){
                return UNKNOWN;
            } else{
                return OFF;
            }
    }
}

bool updateGate(WireStates &ws, const Gate &g){
    WireState i1 = getState(ws, g.input1);
    WireState i2 = getState(ws, g.input2);

    WireState newOutput = performOperation(g.op, i1, i2);
    WireState out = getState(ws, g.output);
    if(newOutput == out){  //no change
        return false;
    }

    ws[g.output] = newOutput;
    return true;
}

uint8_t toDigit(char c){
    return c - '0';
}

constexpr int numOut = 46;
constexpr int numIn = 45;

int main(){
    vector<string> lines = readLines();
    State state = getStartingState(lines);

    bool changed = true;
    while(changed){
        changed = false;
        for(Gate g : state.gates){
            changed = changed || updateGate(state.wires, g);
        }
    }

    for(int i = numOut - 1; i >= 0; i--){

        WireState s = getState(state.wires, string("z") + (i < 10 ? "0" : "") + to_string(i), OFF);
        cout << s - 1;
    }
    cout << endl;
}