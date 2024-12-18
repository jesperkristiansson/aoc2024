#include <iostream>
#include <iomanip>
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

using digit = uint8_t;

struct block{
    int size;
    int idx;

    block(int size, int idx) : size(size), idx(idx) {};
};

struct file{
    int size;
    int idx;
    int id;

    file(int size, int idx, int id) : size(size), idx(idx), id(id) {};
};

struct Disk{
    vector<block> freeBlocks;
    vector<file> files;
};

digit toDigit(char c){
    return c - '0';
}

vector<digit> toDiskMap(string line){
    vector<digit> diskMap;
    diskMap.reserve(line.size());

    for(char c : line){
        digit digit = toDigit(c);
        diskMap.push_back(digit);
    }

    return diskMap;
}

Disk createDisk(const vector<digit> &diskMap){
    Disk disk;

    disk.freeBlocks.reserve(diskMap.size()/2);

    disk.files.reserve(diskMap.size()/2+1);

    bool isFile = true;
    int fileId = 0;
    int currentIdx = 0;
    for(digit d : diskMap){
        if(isFile){     //we assume a file cannot have size == 0
            disk.files.emplace_back(d, currentIdx, fileId++);
        } else if(d > 0){
            disk.freeBlocks.emplace_back(d, currentIdx);
        }
        isFile = !isFile;
        currentIdx += d;
    }

    return disk;
}

void compactFiles(Disk &disk){
    for(int file_i = disk.files.size() - 1; file_i >= 0; file_i--){
        auto &file = disk.files[file_i];
        for(auto &freeBlock : disk.freeBlocks){
            if(freeBlock.idx > file.idx){
                break;
            }

            if(freeBlock.size >= file.size){
                file.idx = freeBlock.idx;

                freeBlock.size -= file.size;
                freeBlock.idx += file.size;
                break;
            }
        }
    }
}

// sum of: start + (start + 1) + ... + (start + (n-1))
//  n numbers total
long long naturalSum(long start, long n){
    return start*n + (n*(n-1))/2;
}

long long checksum(const vector<file> &files){
    long long sum = 0;

    for(const file &file : files){
        sum += file.id*naturalSum(file.idx, file.size);
    }

    return sum;
}

int main(){
    string line;
    getline(cin, line);

    vector<digit> diskMap = toDiskMap(line);
    Disk disk = createDisk(diskMap);

    compactFiles(disk);
    long long sum = checksum(disk.files);

    cout << "sum: " << sum << endl;
}