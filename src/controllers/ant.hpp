#pragma once

#include <sys/types.h>
#include "models/space.hpp"
#include <vector>
#include <array>
// #include "models/grid.hpp"

class Cell;

class Colony;

class Ant
{
public:
    Ant(Colony* colony, Coordinates coordinates);
    Coordinates get_location();
    bool has_sugar();
    void add_sugar();
    void remove_sugar();
    Coordinates find_move();
    const Colony* colony; 

private:
    Coordinates location;
    bool sugar;
};

class Colony
{
public:
    Colony(std::array<Cell*, 4> cells);
    template <class Function>
    void map_ants(Function &&function);
    std::array<Cell*, 4> get_cells();

private:
    const std::array<Cell*, 4> cells;
    std::vector<Ant> ants;
};

#include "controllers/ant.tpp"