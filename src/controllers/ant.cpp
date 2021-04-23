#include "ant.hpp"

std::mt19937 gen(std::random_device{}());

std::size_t random_index(std::size_t lower, std::size_t higher)
{
    std::uniform_int_distribution<std::size_t> dist(lower, higher);
    return dist(gen);
}

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