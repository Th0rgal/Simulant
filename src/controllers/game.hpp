#pragma once

#include "models/grid.hpp"
#include "view/view.hpp"

class Game
{
public:
    Game();
    void start();

private:
    void loop(unsigned long delay);
    View view;
    Grid grid;
};