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
};

bool is_closer_from_nest(Cell base, Cell compared);
