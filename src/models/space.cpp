#include "space.hpp"
#include "math.h"

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
        throw std::invalid_argument("Incorect value for x coordinate");
    if (y < y_min or y > y_max)
        throw std::invalid_argument("Incorect value for y coordinate");
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

bool Coordinates::operator!=(const Coordinates &other) const
{
    return !(*this == other);
}

Coordinates Coordinates::operator+(const Vector &other) const
{
    return Coordinates{x + other.x, y + other.y};
}

Coordinates Coordinates::operator-(const Vector &other) const
{
    return Coordinates{x - other.x, y - other.y};
}

double Coordinates::square_distance_to(double other_x, double other_y)
{
    return (x - other_x) * (x - other_x) + (y - other_y) * (y - other_y);
}

double Coordinates::distance_to(double other_x, double other_y)
{
    return sqrt(square_distance_to(other_x, other_y));
}

double Coordinates::square_distance_to(Coordinates location)
{
    return square_distance_to(location.x, location.y);
}

std::vector<Coordinates> Coordinates::get_neighbors()
{
    std::vector<Coordinates> coordinates;
    if (y - 1 >= Y_MIN)
        coordinates.emplace_back(x, y - 1);
    if (y + 1 <= Y_MAX)
        coordinates.emplace_back(x, y + 1);
    if (x - 1 >= X_MIN)
        coordinates.emplace_back(x - 1, y);
    if (x + 1 <= X_MAX)
        coordinates.emplace_back(x + 1, y);
    return coordinates;
}

std::ostream &operator<<(std::ostream &flux, const Coordinates &c)
{
    return flux << "{ " << c.x << ", " << c.y << " }";
};

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