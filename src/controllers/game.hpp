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
    void apply_ant_logic(Ant * ant, Cell *next_cell, std::vector<Ant*> &killed, std::vector<Ant*> &in_fight);

    void display_not_moving_ant(Ant *ant);
    void move_ant_on_view(Ant *ant, Coordinates location);
};