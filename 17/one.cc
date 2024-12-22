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

using Operand = uint8_t;

enum class Operator : uint8_t{
    ADV = 0,
    BXL = 1,
    BST = 2,
    JNZ = 3,
    BXC = 4,
    OUT = 5,
    BDV = 6,
    CDV = 7
};

struct Instruction{
    Operator op;
    Operand operand;
};

struct Program{
    int A{0},B{0},C{0};
    vector<Instruction> instructions;
};

uint8_t toDigit(char c){
    return c - '0';
}

Program getProgram(vector<string> lines){
    Program program;

    program.A = strtol(lines[0].c_str() + 12, nullptr, 10);
    program.B = strtol(lines[1].c_str() + 12, nullptr, 10);
    program.C = strtol(lines[2].c_str() + 12, nullptr, 10);

    program.instructions.reserve((lines[4].size() - 8)/4);
    for(size_t i = 9; i < lines[4].size(); i += 4){
        Instruction ins;
        ins.op = static_cast<Operator>(toDigit(lines[4][i]));
        ins.operand = static_cast<Operand>(toDigit(lines[4][i+2]));
        program.instructions.push_back(ins);
    }

    return program;
}

int getComboOperand(Operand oper, const Program &program){
    if(oper <= 3){
        return static_cast<int>(oper);
    } else if(oper == 4){
        return program.A;
    } else if(oper == 5){
        return program.B;
    } else if(oper == 6){
        return program.C;
    } else{
        cout << "undefined combo operand " << static_cast<int>(oper) << endl;
        return -1;
    }
}

vector<Operand> runProgram(Program &program){
    vector<Operand> output;

    for(int programCounter = 0; programCounter < (int)program.instructions.size(); programCounter++){
        const Instruction &ins = program.instructions[programCounter];
        switch(ins.op){
            case Operator::ADV:
                program.A = program.A >> getComboOperand(ins.operand, program);
                break;
            case Operator::BXL:
                program.B = program.B ^ ins.operand;
                break;
            case Operator::BST:
                program.B = getComboOperand(ins.operand, program) & 0b111;
                break;
            case Operator::JNZ:
                if(program.A != 0){
                    programCounter = ins.operand/2 - 1;
                }
                break;
            case Operator::BXC:
                program.B = program.B ^ program.C;
                break;
            case Operator::OUT:
                output.push_back(getComboOperand(ins.operand, program) & 0b111);
                break;
            case Operator::BDV:
                program.B = program.A >> getComboOperand(ins.operand, program);
                break;
            case Operator::CDV:
                program.C = program.A >> getComboOperand(ins.operand, program);
                break;
            default:
                cout << "Unknown operator " << static_cast<uint8_t>(ins.op) << endl;
        }
    }
    return output;
}

int main(){
    vector<string> lines = readLines();
    Program program = getProgram(lines);
    vector<Operand> output = runProgram(program);
    if(output.size() > 0){
        cout << (char)(output[0] + '0');
        for(size_t i = 1; i < output.size(); i++){
            cout << ',' << (char)(output[i] + '0');
        }
    }
    cout << endl;
}