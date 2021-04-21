#pragma once

#include <vector>
#include <sys/types.h>
#include "models/space.hpp"

struct Cell
{
    Cell(const Coordinates location);
    Coordinates get_location();
    bool is_void();

    unsigned short get_sugar_pheromons();
    bool has_sugar();
    bool is_sugar_track();
    void add_sugar();
    void remove_sugar();
    void add_sugar_pheromon();
    void reduce_sugar_pheromon();

    unsigned short get_nest_pheromons();
    bool is_nest();
    void set_nest();

    ssize_t get_ant_index();
    void set_ant(size_t antIndex);
    void remove_ant();
};

struct Grid
{
    Grid();
    std::vector<Coordinates> get_neighbors(const Coordinates &location);
    Cell load_cell(const Coordinates &location); //copy
    void add_cell(Cell cell);
    void linearize_nest_pheromons();
    void reduce_sugar_pheromons();
};