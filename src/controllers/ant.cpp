#include "ant.hpp"

Ant::Ant(Coordinates coordinates, size_t index) : location(coordinates), index(index), sugar(false)
{
}

Coordinates Ant::get_location()
{
    return location;
}

size_t Ant::get_index()
{
    return index;
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

void Ant::move()
{
    // todo
}