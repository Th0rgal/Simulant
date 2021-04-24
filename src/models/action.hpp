#pragma once

#include <vector>
#include "models/grid.hpp"

enum struct ActionType {
    ant_move,
    sugar_consume,
};

class Action {
    ActionType type;
    std::vector<Cell*> state_begin;
    std::vector<Cell*> state_end;
};