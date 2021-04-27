#pragma once

#include <sys/types.h>
#include "models/space.hpp"
#include <vector>
#include <array>

class Grid;

class Cell;

class Colony;

class Ant
{
public:
    Ant(Colony *colony, Coordinates coordinates);
    Coordinates get_location();
    bool has_sugar();
    void add_sugar();
    void deposit_sugar();
    void move(Grid &grid, Cell *new_cell);
    std::vector<Coordinates> find_moves(Grid &grid);
    Colony *get_colony();

private:
    Coordinates location;
    bool sugar;
    Colony *colony;
};

class Colony
{
public:
    Colony(std::array<Cell *, 4> cells);
    ~Colony();
    template <class Function>
    void map_ants(Function &&function);
    std::array<Cell *, 4> get_cells();
    double centroid_x;
    double centroid_y;

    int left_corner_x;
    int left_corner_y;

    void add_sugar();
    size_t spawn_ants();

    void remove_ant(Grid &grid, size_t ant_id);
    void add_ant(Ant *ant);
    size_t find_ant_index(Ant *);

private:
    const std::array<Cell *, 4> cells;
    std::vector<Ant *> ants;
    size_t sugar;
};

#include "controllers/ant.tpp"