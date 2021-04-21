#include "space.hpp"

Coordinates::Coordinates(int x, int y) : x(x), y(y)
{
}

std::size_t std::hash<Coordinates>::operator()(const Coordinates &coordinates) const noexcept
{
    std::hash<std::size_t> size_t_hash;
    return size_t_hash(coordinates.x) ^ size_t_hash(coordinates.y);
}

bool Coordinates::operator==(const Coordinates &other) const
{
    return x == other.x && y == other.y;
}

Coordinates Coordinates::operator+(const Vector &other)
{
    return Coordinates{x + other.x, y + other.y};
}

Coordinates Coordinates::operator+=(const Vector &other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Coordinates Coordinates::operator-(const Vector &other)
{
    return Coordinates{x - other.x, y - other.y};
}

Coordinates Coordinates::operator-=(const Vector &other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}



Vector::Vector(int x, int y) : x(x), y(y)
{
}

std::size_t std::hash<Vector>::operator()(const Vector &vector) const noexcept
{
    std::hash<std::size_t> size_t_hash;
    return size_t_hash(vector.x) ^ size_t_hash(vector.y);
}

bool Vector::operator==(const Vector &other) const
{
    return x == other.x && y == other.y;
}

Vector Vector::operator+(const Vector &other)
{
    return Vector{x + other.x, y + other.y};
}

Vector Vector::operator+=(const Vector &other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vector Vector::operator-(const Vector &other)
{
    return Vector{x - other.x, y - other.y};
}

Vector Vector::operator-=(const Vector &other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector Vector::operator*(int factor)
{
    return Vector{x * factor, y * factor};
}

Vector Vector::operator*=(int factor)
{
    x *= factor;
    y *= factor;
    return *this;
}

Vector Vector::operator/(int factor)
{
    return Vector{x / factor, y / factor};
}

Vector Vector::operator/=(int factor)
{
    x /= factor;
    y /= factor;
    return *this;
}
