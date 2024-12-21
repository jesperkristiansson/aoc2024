#pragma once

#include <vector>

struct Direction{
    Direction() = default;
    Direction(int dx, int dy) : dx{dx}, dy{dy} {}

    int dx{0}, dy{0};
};

struct Point{
    Point() = default;
    Point(int x, int y) : x{x}, y{y} {}

    auto operator<=>(const Point &other) const = default;

    Point operator+(const Direction &d) const{
        return Point(x + d.dx, y + d.dy);
    }
    Point &operator+=(const Direction &d){
        x += d.dx;
        y += d.dy;
        return *this;
    }
    Direction operator-(const Point &other){
        return Direction(other.x - this->x, other.y - this->y);
    }

    int x{0}, y{0};
};

template <typename T>
struct Grid{
    Grid() = default;
    Grid(int xMax, int yMax) : mat(yMax, std::vector<T>(xMax)), xMax{xMax}, yMax{yMax}{}

    T &at(const Point &p){
        return mat[p.y][p.x];
    }
    const T &at(const Point &p) const{
        return mat[p.y][p.x];
    }
    bool contains(const Point &p){
        return p.x >= 0 && p.x < xMax && p.y >= 0 && p.y < yMax;
    }

    std::vector<std::vector<T>> mat{};
    int xMax{0}, yMax{0};
};