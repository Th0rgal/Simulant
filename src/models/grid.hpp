#pragma once

#include <vector>
#include <iostream>
#include <sys/types.h>
#include "models/space.hpp"
#include "controllers/ant.hpp"
#include <array>

class Cell
{
public:
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

    bool is_nest();
    void set_nest();
    unsigned short get_nest_pheromons();

    ssize_t get_ant_index();
    void set_ant(size_t antIndex);
    void remove_ant();

private:
    Coordinates location;
    bool sugar;
    unsigned short sugar_pheromon;
    unsigned short nest_pheromon;
    Ant *ant;
};

class Grid
{
public:
    Grid(); //construit la map
    void linearize_nest_pheromons();
    void reduce_sugar_pheromons();
    template <class Function>
    void map_ants(Function &&function);

private:
    std::vector<Ant> ants;
    std::array<Cell *, SPACE_WIDTH * SPACE_HEIGHT> map;
};

// class Nest {
// 	public:
// 		Nest();

// 	private:
//		Coordinates				cor;
// 		std::unordered_set<Ant>	ants;
// 		uint8_t					color;
//}

bool is_closer_from_nest(Cell base, Cell compared);

#include "models/grid.tpp"