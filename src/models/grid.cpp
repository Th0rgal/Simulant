#include "models/grid.hpp"

std::ostream &operator<<(std::ostream &out, const std::unordered_set<Coordinates> &coordinates_set)
{
    out << "{";
    for (const Coordinates &coordinates : coordinates_set)
        out << coordinates;
    return out << "}";
}
/*
std::unordered_set<Coordinates> Grid::get_neighbors(const Coordinates &c) const
{
    std::unordered_set<Coordinates> neighbors;

    int x_min = std::max(c.get_x() - 1, X_MIN);
    int y_min = std::max(c.get_y() - 1, Y_MIN);
    int x_max = std::min(c.get_x() + 1, X_MAX + 1);
    int y_max = std::min(c.get_y() + 1, Y_MAX + 1);
    for (int y = y_min; y < y_max; y++) {
        for (int x = x_min; x < x_max; x++) {
            neighbors.emplace();
        }
    }
}*/

Cell::Cell(Coordinates location) : location(location)
{
}

Grid::Grid()
{
    for (int i = 0; i < SPACE_WIDTH * SPACE_HEIGHT; i++)
        map[i] = new Cell(Coordinates(i % SPACE_WIDTH, i / SPACE_WIDTH));
}
