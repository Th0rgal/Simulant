#pragma once

#include <vector>
#include "models/grid.hpp"

enum struct ActionType {
    AntMove,
    AntDeath,
    SugarConsume,
};

struct Action {
    ActionType type;
    std::vector<Coordinates> updated;
    Colony* colony;
};