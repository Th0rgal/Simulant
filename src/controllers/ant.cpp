#include "controllers/ant.hpp"
#include "controllers/random.hpp"

Ant::Ant(Coordinates coordinates) : location(coordinates), sugar(false)
{
}

Coordinates Ant::get_location()
{
    return location;
}

bool Ant::has_sugar()
{
    return sugar;
}

void Ant::add_sugar()
{
    sugar = true;
}

void Ant::remove_sugar()
{
    sugar = false;
}

Coordinates Ant::find_move()
{
    std::vector<Coordinates> neighbors = location.get_neighbors();
    return neighbors[random_index(0, neighbors.size()-1)];
}

Colony::Colony(std::array<Cell*, 4> cells) : cells(cells)
{

}

std::array<Cell*, 4> Colony::get_cells() {
    return cells;
}