#pragma once

#include <bits/stdc++.h>

#define SPACE_WIDTH 21
#define SPACE_HEIGHT 21

struct Vector
{
    Vector(int x, int y);
    int x;
    int y;

    /**
     * to compare two Vector objects (required for unordered_set)
     * 
     * @param const Vector &other, another instance of Vector
     *
     * @author Thomas Marchand
     **/
    bool operator==(const Vector &other) const;
    Vector operator+(const Vector &other);
    Vector operator+=(const Vector &other);
    Vector operator-(const Vector &other);
    Vector operator-=(const Vector &other);
    Vector operator*(const int factor);
    Vector operator*=(const int factor);
    Vector operator/(const int factor);
    Vector operator/=(const int factor);
};

/**
 * to define Vector hash (required in order to use unordered_set)
 * 
 * @param const Vector &vector a Vector instance
 *
 * @author Thomas Marchand
 **/
template <>
struct std::hash<Vector>
{
    std::size_t operator()(const Vector &vector) const noexcept;
};

struct Coordinates
{
    Coordinates(int x, int y);
    int x;
    int y;

    /**
     * to compare two Coordinates objects (required for unordered_set)
     * 
     * @param const Coordinates &other, another instance of Coordinates
     *
     * @author Thomas Marchand
     **/
    bool operator==(const Coordinates &other) const;
    Coordinates operator+(const Vector &other) const;
    Coordinates operator-(const Vector &other) const;

    int get_x();
    int get_y();
};

/**
 * to define Coordinates hash (required in order to use unordered_set)
 * 
 * @param const Coordinates &coordinates a Coordinate instance
 *
 * @author Thomas Marchand
 **/
template <>
struct std::hash<Coordinates>
{
    std::size_t operator()(const Coordinates &coordinates) const noexcept;
};
