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
#include <functional>

#include "readInput.hpp"

using namespace std;

/*  State  */
static bool enabled = true;
static long sum = 0;

/*  Operations  */
static const char MUL_PREFIX[] = "mul";
static const char DO_PREFIX[] = "do";
static const char DONT_PREFIX[] = "don't";

struct op{
    const char *identifier;
    function<const char *(const char *)> handler;
};

const char *handleDo(const char *current_p){
    if(*current_p != '('){
        return current_p;
    }
    ++current_p;

    if(*current_p != ')'){
        return current_p;
    }
    ++current_p;

    enabled = true;
    return current_p;
}

const char *handleDont(const char *current_p){
    if(*current_p != '('){
        return current_p;
    }
    ++current_p;

    if(*current_p != ')'){
        return current_p;
    }
    ++current_p;

    enabled = false;
    return current_p;
}

const char *handleMul(const char *current_p){
    if(*current_p != '('){
        return current_p;
    }
    ++current_p;

    if(!isdigit(*current_p)){
        return current_p;
    }

    char *end_p;
    long num1 = strtol(current_p, &end_p, 10);
    current_p = end_p;

    if(*current_p != ','){
        return current_p;
    }
    current_p++;

    if(!isdigit(*current_p)){
        return current_p;
    }

    long num2 = strtol(current_p, &end_p, 10);
    current_p = end_p;

    if(*current_p != ')'){
        return current_p;
    }
    current_p++;

    if(enabled){
        sum += num1*num2;
    }

    return current_p;
}

op ops[] = {
    {.identifier = MUL_PREFIX, .handler = handleMul},
    {.identifier = DO_PREFIX, .handler = handleDo},
    {.identifier = DONT_PREFIX, .handler = handleDont}
};

void handleLine(const string &line){
    const char *current_p = line.c_str();

    struct foundOp{
        struct op op;
        const char *location;
    };

    while(true){
        foundOp firstOp = {.location = NULL};
        for(const auto &op : ops){
            size_t id_len = strlen(op.identifier);
            char target[id_len + 2];
            strcpy(target, op.identifier);
            target[id_len] = '(';
            target[id_len + 1] = '\0';

            const char *found_p = strstr(current_p, target);
            if(found_p == NULL){
                continue;
            }

            if(firstOp.location == NULL || found_p < firstOp.location){
                firstOp = {.op = op, .location = found_p};
            }
        }

        if(firstOp.location == NULL){       //no more operations
            return;
        }
        current_p = firstOp.op.handler(firstOp.location + strlen(firstOp.op.identifier));
    }
}

int main(){
    vector<string> lines = readLines();

    for(const auto &line : lines){
        handleLine(line);
    }

    cout << sum << endl;
}