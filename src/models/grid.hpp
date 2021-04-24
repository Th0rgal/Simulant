#pragma once

#include <vector>
#include <iostream>
#include <sys/types.h>
#include "models/space.hpp"
#include "controllers/ant.hpp"
#include <array>
#include <unordered_set>
#include <map>

class Cell
{
public:
    Cell(const Coordinates location);
    Coordinates get_location() const;

    bool is_void();

    bool is_sugar_track();

    bool has_sugar();
    void add_sugar();
    void remove_sugar();

    bool is_nest();
    void set_nest(Colony *colony);
    Colony *get_nest();

    bool has_ant();
    void set_ant(Ant *ant);
    void remove_ant();
    Ant *get_ant();

    double get_nest_pheromons(const Colony *colony);

    double get_sugar_pheromons();
    void add_sugar_pheromon();
    void reduce_sugar_pheromon();

private:
    Coordinates location;
    bool sugar;
    double sugar_pheromons = 0;
    std::map<const Colony *, double> nest_pheromons;
    Colony *nest = NULL;
    Ant *ant = NULL;

    friend class Grid;
};

class Grid
{
public:
    Grid(size_t colonies_amount); //construit la map
    void linearize_nest_pheromons();
    void reduce_sugar_pheromons();
    template <class Function>
    void map_colony(Function &&function);
    template <class Function>
    void map_ants(Function &&function);
    Cell *get_cell(int x, int y);
    Cell *get_cell(Coordinates location);

private:
    std::array<Cell *, 4> find_nest_cells();
    std::array<Cell *, SPACE_WIDTH * SPACE_HEIGHT> map;
    std::vector<Colony *> colonies;
    void spawn_ants(Colony* colony, int start_x, int start_y);

    friend class View;
};

#include "models/grid.tpp"