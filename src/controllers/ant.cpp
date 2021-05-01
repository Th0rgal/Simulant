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

void Ant::deposit_sugar()
{
    sugar = false;
    colony->add_sugar();
}

void Ant::move(Grid &grid, Cell *new_cell)
{
    grid.get_cell(location)->set_ant(NULL);
    new_cell->set_ant(this);
    location = new_cell->get_location();
}

Colony *Ant::get_colony()
{
    return colony;
}

std::map<Coordinates, double> Ant::find_moves(Grid &grid)
{
    std::vector<Coordinates> neighbors = location.get_neighbors();
    std::map<Coordinates, double> output;
    double maximum = 0;
    for (const Coordinates &neighbor : neighbors)
    {
        Cell *cell = grid.get_cell(neighbor);
        if ((!cell->has_sugar()))
        {
            double score = cell->get_nest_pheromons(colony);
            output[neighbor] = score;
            maximum += score;
        }
        else if (!has_sugar() && (!cell->has_ant() || cell->get_ant()->colony != colony))
        {
            double score = 1 - cell->get_nest_pheromons(colony);
            output[neighbor] = score;
            maximum += score;
        }
    }
    for (auto iterator = output.begin(); iterator != output.end(); iterator++)
    {
        iterator->second /= maximum;
    }
    return output;
}

Colony::Colony(std::array<Cell *, 4> cells) : cells(cells)
{
    left_corner_x = cells[0]->get_location().x;
    left_corner_y = cells[0]->get_location().y;

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

Colony::~Colony()
{
    for (Ant *ant : ants)
        delete (ant);
}

std::array<Cell *, 4> Colony::get_cells()
{
    return cells;
}

void Colony::remove_ant(Grid &grid, size_t ant_id)
{
    Ant *ant = ants[ant_id];
    Cell *cell = grid.get_cell(ant->get_location());
    if (cell->get_ant() == ant)
        cell->set_ant(NULL);
    delete ant;
    if (ant_id < ants.size())
        ants.erase(ants.begin() + ant_id);
}

size_t Colony::find_ant_index(Ant *ant)
{
    for (size_t i = 0; i < ants.size(); i++)
        if (ant == ants[i])
            return i;
    throw std::invalid_argument("This ant does not belong to the colony");
}

void Colony::add_ant(Ant *ant)
{
    ants.push_back(ant);
}

void Colony::add_sugar()
{
    sugar += 1;
}

size_t Colony::spawn_ants()
{
    return sugar / 10;
    sugar %= 10;
}