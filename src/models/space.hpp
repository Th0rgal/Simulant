#pragma once

#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>

#define SPACE_WIDTH 64
#define SPACE_HEIGHT 64

#define X_MIN ((-SPACE_WIDTH / 2))
#define Y_MIN ((-SPACE_HEIGHT / 2))
#define X_MAX ((SPACE_WIDTH / 2 - (SPACE_WIDTH % 2 == 0 ? 1 : 0)))
#define Y_MAX ((SPACE_HEIGHT / 2 - (SPACE_HEIGHT % 2 == 0 ? 1 : 0)))

struct Vector
{
    Vector(int x, int y);
    int x;
    int y;

    /**
     * to compare two Vector objects (required for set)
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
 * to define Vector hash (required in order to use set)
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

    double square_distance_to(Coordinates location);
    double distance_to(double x, double y);
    double square_distance_to(double x, double y) const;
    std::vector<Coordinates> get_neighbors();

    /**
     * to compare two Coordinates objects (required for set)
     * 
     * @param const Coordinates &other, another instance of Coordinates
     *
     * @author Thomas Marchand
     **/
    bool operator==(const Coordinates &other) const;
    bool operator!=(const Coordinates &other) const;
    bool operator<(const Coordinates &other) const;
    Coordinates operator+(const Vector &other) const;
    Coordinates operator-(const Vector &other) const;
};

std::ostream &operator<<(std::ostream &flux, const Coordinates &c);

/**
 * to define Coordinates hash (required in order to use set)
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
