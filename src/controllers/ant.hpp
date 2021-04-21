#pragma once

#include <stdlib.h>
#include "models/space.hpp"

class Ant
{
public:
    Ant(Coordinates coordinates, size_t index);
    Coordinates get_location();
    size_t get_index();
    bool has_sugar();
    void add_sugar();
    void remove_sugar();
    void move();

private:
    Coordinates location;
    size_t index;
    bool sugar;
};

bool is_closer_from_nest(Cell base, Cell compared);
