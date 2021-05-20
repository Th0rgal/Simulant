#pragma once

#include "models/space.hpp"
#include <sys/types.h>
#include <vector>
#include <array>
#include <map>

class Grid;

class Cell;

class Colony;

class Ant
{
public:
    /**
     * Ant constructor
     * 
     * @param Colony* ant's colony
     * @param Coordinates ant's location
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    Ant(Colony *colony, Coordinates coordinates);

    /**
     * Getter for ant's location
     * 
     * @return Coordinates ant's location
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    Coordinates get_location();

    /**
     * Setter for ant's location
     * 
     * @param const Coordinates& new ant's location
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    void set_location(const Coordinates &new_location);

    /**
     * Getter for ant's colony
     * 
     * @return Colony* pointer to ant's colony
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    Colony *get_colony();

    /**
     * Getter for ant's sugar
     * 
     * @return bool does it have sugar?
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    bool has_sugar();

    /**
     * Set ant sugar to true
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    void add_sugar();

    /**
     * Deposit sugar
     * 
     * @return bool true if a baby ant must be spawned
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    bool deposit_sugar();

    /**
     * Move ant to a new cell
     * 
     * @param Grid& a reference to the grid
     * @param Cell* a pointer to the destination
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    void move(Grid &grid, Cell *new_cell);

    /**
     * Find best cell for next movement
     * @warning calling this method will refresh adjacent cells
     * 
     * @param Grid& a reference to the grid
     * @param size_t current game tick
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    Cell *find_move(Grid &grid, size_t current_tick);

private:
    Coordinates location;
    bool sugar;
    Colony *colony;
    double orientation;
    double origin_x;
    double origin_y;
};

class Colony
{
public:
    /**
     * Colony constructor
     * 
     * @param std::array<Cell *, 4> colony cells
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    Colony(std::array<Cell *, 4> cells);

    /**
     * Colony destructor
     * 
     * @confidence 3
     * @author Frédéric Becerril
     **/
    ~Colony();

    /**
     * Colony getter nb ants
     * 
     * @confidence 5
     * @author Frédéric Becerril
     **/
    int get_ants_amount();

    template <class Function>
    /**
     * Utility function to apply a lambda function over all ants of the colony
     * 
     * @param Function&& a lambda function with Ant* as parameter
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    void map_ants(Function &&function);

    /**
     * Getter for colony cells
     * 
     * @return std::array<Cell *, 4> colony cells
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    std::array<Cell *, 4> get_cells();

    /**
     * Add sugar to the colony
     * 
     * @return bool true if a baby ant must be spawned
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    bool add_sugar();

    /**
     * Remove ant from the colony
     * 
     * @param Grid& a reference to the grid
     * @param size_t index of the ant within the colony vectors
     * 
     * @confidence 3
     * @author Thomas Marchand
     **/
    void remove_ant(Grid &grid, size_t ant_id);

    /**
     * Add ant to the colony
     * 
     * @warning Use Grid::spawn_ant to add the ant to the colony AND the grid
     * 
     * @param Ant* a pointer to the ant to add
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    void add_ant(Ant *ant);

    /**
     * Find ant index within the colony vector
     * 
     * @warning The provided ant must belong to the colony
     * @param Ant* a pointer to the ant to look for
     * @return size_t index of the ant
     * 
     * @confidence 5
     * @author Thomas Marchand
     **/
    size_t find_ant_index(Ant *);
    double centroid_x;
    double centroid_y;
    int left_corner_x;
    int left_corner_y;

private:
    const std::array<Cell *, 4> cells;
    std::vector<Ant *> ants;
    size_t sugar = 0;
};

#include "controllers/ant.tpp"