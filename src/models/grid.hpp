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
    Ant *get_ant();

    double get_nest_pheromons(const Colony *colony);

    double get_sugar_pheromons();
    void add_sugar_pheromon();
    void reduce_sugar_pheromon();

    void update(size_t current_block);

private:
    Coordinates location;
    bool sugar = false;
    double sugar_pheromons = 0;
    std::map<const Colony *, double> nest_pheromons;
    Colony *nest = NULL;
    Ant *ant = NULL;
    size_t last_block = 0;

    friend class Grid;
    friend class Colony;
};

class Grid
{
public:
    Grid(size_t colonies_amount); //construit la map
    template <class Function>
    void map_colony(Function &&function);
    template <class Function>
    void map_ants(Function &&function);
    Cell *get_cell(const Vector &vector) const;
    Cell *get_cell(int x, int y) const;
    Cell *get_cell(Coordinates location, size_t current_block = 0) const;
    void set_ant(Ant *ant, Coordinates coordinates);
    void spawn_ant(Colony *colony, Coordinates location);
    void clear();

private:
    std::array<Cell *, SPACE_WIDTH * SPACE_HEIGHT> map;
    std::vector<Colony *> colonies;

    std::array<Cell *, 4> find_nest_cells();
    void summon_ants(Colony *colony);
    void summon_sugars(size_t amount);
    Cell *find_empty_cell();

    friend class View;
};

#include "models/grid.tpp"