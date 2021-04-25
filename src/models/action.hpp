#pragma once

#include <vector>
#include "models/grid.hpp"

enum struct ActionType {
    AntMove,
    SugarConsume,
};

struct Action {
    ActionType type;
    std::vector<Cell*> updated;
};