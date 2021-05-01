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

double fixed_min_max(double x, double min, double max)
{
    return (max - min > 0) ? (x - min) / (max - min) : 0;
}

std::map<Coordinates, double> Ant::find_moves(Grid &grid)
{
    std::vector<Coordinates> neighbors = location.get_neighbors();
    std::vector<Cell *> possible_cells;
    double min_pheromon = 1;
    double max_pheromon = 0;
    for (const Coordinates &neighbor : neighbors)
    {
        Cell *cell = grid.get_cell(neighbor);
        if ((!cell->has_sugar() || !has_sugar()) && (!cell->has_ant() || cell->get_ant()->colony != colony))
        {
            possible_cells.push_back(cell);
            double pheromon = cell->get_nest_pheromons(colony);
            if (pheromon < min_pheromon)
                min_pheromon = pheromon;
            if (pheromon > max_pheromon)
                max_pheromon = pheromon;
        }
    }

    std::map<Coordinates, double> output;
    double average_probability = 1.0 / possible_cells.size();
    double total_score = 0;
    for (Cell *cell : possible_cells)
    {
        double score = 0.1; // the base score: used to increase randomness
        if (has_sugar())
            score += fixed_min_max(cell->get_nest_pheromons(colony), min_pheromon, max_pheromon);
        else
            score += 1 - fixed_min_max(cell->get_nest_pheromons(colony), min_pheromon, max_pheromon); // normalement on devrait ici chercher le sucre
        output[cell->get_location()] = score;
        total_score += score;
    }

    for (auto iterator = output.begin(); iterator != output.end(); iterator++)
        iterator->second / total_score;

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

void Colony::spawn_ants()
{

    size_t to_spawn = sugar / 1;
    sugar %= 1;

    for (size_t i = to_spawn + 1; i > 1; i--)
        spawn_ant();
}

void Colony::spawn_ant()
{
    std::cout << "Ant spawned" << std::endl;
}