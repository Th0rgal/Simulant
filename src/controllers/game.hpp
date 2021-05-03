#pragma once

#include "models/grid.hpp"
#include "view/view.hpp"

class Game
{
public:
    Game();
    void start();
    void restart();

private:
    View view;
    Grid grid;
    std::vector<Action> delta;
    std::map<Colony *, std::vector<Coordinates>> spawn_list;
    void loop(unsigned long delay, size_t current_block);
};