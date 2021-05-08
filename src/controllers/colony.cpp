#include "controllers/ant.hpp"
#include "models/grid.hpp"

void Colony::remove_ant(Grid &grid, size_t ant_id)
{
    Ant *ant = ants[ant_id];
    Cell *cell = grid.get_cell(ant->get_location());
    if (cell->get_ant() == ant)
        cell->set_ant(NULL);
    delete ant;
    ant = NULL;
    if (ant_id < ants.size())
        ants.erase(ants.begin() + ant_id);
}

size_t Colony::find_ant_index(Ant *ant)
{
    for (size_t i = 0; i < ants.size(); i++)
        if (ant == ants[i])
            return i;
    std::cout << "ANT:" << ant << std::endl;
    std::cout << "location:" << ant->get_location() << std::endl;
    std::cout << "colony:" << ant->get_colony() << std::endl;
    std::cout << "expected colony:" << this << std::endl;
    throw std::invalid_argument("This ant does not belong to the colony");
}

void Colony::add_ant(Ant *ant)
{
    ants.push_back(ant);
}

bool Colony::add_sugar()
{
    sugar += 1;
    if (sugar > 1)
    {
        sugar -= 1;
        return true;
    }
    return false;
}
