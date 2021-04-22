#include "space.hpp"

Coordinates::Coordinates(int x, int y) : x(x), y(y)
{
    int x_min = -SPACE_WIDTH / 2;
    int x_max = SPACE_WIDTH / 2;
    int y_min = -SPACE_HEIGHT / 2;
    int y_max = SPACE_HEIGHT / 2;

    if (SPACE_WIDTH % 2 == 0)
        x_max--;
    if (SPACE_HEIGHT % 2 == 0)
        y_max--;
    if (x < x_min or x > x_max)
    {
        throw std::invalid_argument("Incorect value for coordinates");
    }
    if (y < y_min or y > y_max)
    {
        throw std::invalid_argument("Incorect value for coordinates");
    }
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

Coordinates Coordinates::operator+(const Vector &other) const
{
    return Coordinates{x + other.x, y + other.y};
}

Coordinates Coordinates::operator-(const Vector &other) const
{
    return Coordinates{x - other.x, y - other.y};
}

int Coordinates::get_x()
{
    return x;
}

int Coordinates::get_y()
{
    return y;
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
