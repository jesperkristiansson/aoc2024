#pragma once

#include <iostream>
#include <vector>
#include <sstream>

std::vector<std::string> readLines(){
    std::vector<std::string> vec;
    while(!std::cin.eof()){
        std::string line;
        getline(std::cin, line);

        vec.push_back(line);
    }

    while(vec.back() == ""){
        vec.erase(vec.begin() + vec.size() - 1);
    }

    return vec;
}

template<typename T>
std::vector<T> readInput_vec(){
    std::vector<T> vec;
    while(!std::cin.eof()){
        T t;
        std::cin >> t;

        if(!std::cin){
            break;
        }

        vec.push_back(t);
    }

    return vec;
}

template<typename T>
std::vector<std::vector<T>> readInput_mat(){
    std::vector<std::vector<T>> mat;
    while(!std::cin.eof()){
        std::string line;
        getline(std::cin, line);
        if(line == ""){
            break;
        }

        mat.emplace_back();
        std::stringstream ss(line);
        while(!ss.eof()){
            T t;
            ss >> t;
            mat.back().push_back(t);
        }
    }

    return mat;
}