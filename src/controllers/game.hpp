#pragma once

#include "models/grid.hpp"

class Game
{
public:
    Game();
    void start();

private:
    void loop(unsigned long delay);
    Grid grid;
};