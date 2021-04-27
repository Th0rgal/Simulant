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
    void loop(unsigned long delay);
};