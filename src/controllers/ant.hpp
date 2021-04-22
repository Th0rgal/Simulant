#pragma once

#include <sys/types.h>
#include "models/space.hpp"

class Ant
{
public:
    Ant(Coordinates coordinates, size_t index);
    Coordinates get_location();
    bool has_sugar();
    void add_sugar();
    void remove_sugar();
    void move();

private:
    Coordinates location;
    bool sugar;
};