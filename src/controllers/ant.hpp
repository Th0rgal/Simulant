#pragma once

#include <sys/types.h>
#include "models/space.hpp"
#include <vector>
#include <random>

class Ant
{
public:
    Ant(Coordinates coordinates);
    Coordinates get_location();
    bool has_sugar();
    void add_sugar();
    void remove_sugar();
    Coordinates find_move();

private:
    Coordinates location;
    bool sugar;
};

class Colony
{
public:
    Colony(){};
    ~Colony(){};
    template <class Function>
    void map_ants(Function &&function);

private:
    std::vector<Ant> ants;
};

#include "controllers/ant.tpp"