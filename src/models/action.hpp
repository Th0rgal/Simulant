#pragma once

#include <vector>
#include "models/grid.hpp"

enum struct ActionType {
    AntMove,
    SugarConsume,
};

class Action {
    ActionType type;
    std::vector<Cell*> updated;
};