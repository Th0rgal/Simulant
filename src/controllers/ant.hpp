#pragma once

#include <sys/types.h>
#include "models/space.hpp"
#include <vector>

class Ant
{
public:
    Ant(Coordinates coordinates, size_t index);
    Coordinates get_location();
    bool has_sugar();
    void add_sugar();
    void remove_sugar();
    void move();

private:
    Coordinates location;
    bool sugar;
};

class Colony 
{
public:
    Colony() {};
    ~Colony() {};
    template <class Function>
    void map_ants(Function &&function);

private:
    std::vector<Ant> ants;
};

#include "controllers/ant.tpp"