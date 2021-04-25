#include "controllers/ant.hpp"
#include "controllers/random.hpp"
#include "models/grid.hpp"

Ant::Ant(Colony *colony, Coordinates coordinates) : colony(colony), location(coordinates), sugar(false)
{
}

Coordinates Ant::get_location()
{
    return location;
}

bool Ant::has_sugar()
{
    return sugar;
}

void Ant::add_sugar()
{
    sugar = true;
}

void Ant::remove_sugar()
{
    sugar = false;
}

Coordinates Ant::find_move()
{
    std::cout << "location:" << location << std::endl;
    std::vector<Coordinates> neighbors = location.get_neighbors();
    for (const Coordinates &neighbor : neighbors)
        std::cout << "neighbord:" << neighbor << std::endl;
    return neighbors[random_index(0, neighbors.size() - 1)];
}

Colony::Colony(std::array<Cell *, 4> cells) : cells(cells)
{
    for (Cell *cell : cells)
    {
        centroid_x += cell->get_location().x;
        centroid_y += cell->get_location().y;
        cell->set_nest(this);
        cell->nest_pheromons[this] = 1;
    }
    centroid_x /= cells.size();
    centroid_y /= cells.size();
}

std::array<Cell *, 4> Colony::get_cells()
{
    return cells;
}

void Colony::remove_ant(Grid &grid, size_t ant_id)
{
    Ant *ant = ants[ant_id];
    grid.get_cell(ant->get_location())->set_ant(NULL);
    delete ant;
    if (ant_id < ants.size())
        ants.erase(ants.begin() + ant_id);
}

void Colony::add_ant(Ant *ant)
{
    ants.push_back(ant);
}