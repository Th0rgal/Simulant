#include "controllers/ant.hpp"
#include "controllers/random.hpp"
#include "models/grid.hpp"
#include "math.h"

double random_orientation()
{
    std::cout << "NEW ORIENTATION" << std::endl;
    return random_double(0, 2 * M_PI);
}

Ant::Ant(Colony *colony, Coordinates coordinates) : colony(colony), location(coordinates), sugar(false), orientation(random_orientation())
{
    std::cout << "a new ant was created with orientation:" << orientation << std::endl;
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

bool Ant::deposit_sugar()
{
    sugar = false;
    return colony->add_sugar();
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
    return (max - min > 0) ? (x - min) / (max - min) : 1;
}

struct MetaCell
{
    Cell *cell;
    double square_distance;
    double orientation;
};

Cell *Ant::find_move(Grid &grid, size_t current_block)
{
    std::vector<MetaCell> cells;
    Cell *sugar = NULL;
    bool pheromons = false;
    for (const Coordinates &coo : location.get_neighbors())
    {
        Cell *cell = grid.get_cell(coo, current_block);
        cells.push_back({cell,
                         coo.square_distance_to(colony->centroid_x, colony->centroid_y),
                         atan2(location.x - colony->centroid_x, location.y - colony->centroid_y)});
        if (cell->has_sugar())
            sugar = cell;
        if (cell->get_sugar_pheromons() > 0)
            pheromons = true;
    }

    if (!has_sugar()) // looking for sugar
    {
        // 1 we found sugar
        if (sugar != NULL)
            return sugar;

        // 2: we follow pheromons
        double current_distance = location.square_distance_to(colony->centroid_x, colony->centroid_y);
        std::vector<MetaCell> distancing;

        if (pheromons)
        {
            Cell *farthest = NULL;
            for (MetaCell meta_cell : cells)
                if (meta_cell.square_distance > current_distance)
                {
                    distancing.push_back(meta_cell);
                    if (meta_cell.cell->get_sugar_pheromons() > (farthest == NULL ? 0 : farthest->get_sugar_pheromons()))
                        farthest = meta_cell.cell;
                }
            return farthest; // todo handle no farthest
        }
        else
            for (MetaCell meta_cell : cells)
                if (meta_cell.square_distance > current_distance)
                    distancing.push_back(meta_cell);

        // 3: we explore the world
        double best_orientation = 10; // magic value
        Cell *best_cell = NULL;
        for (MetaCell cell : distancing)
        {
            std::cout << "YOLO" << std::endl;
            double orientation_shift = abs(cell.orientation - orientation);
            std::cout << orientation_shift << std::endl;
            if (orientation_shift < best_orientation)
            {
                best_orientation = orientation_shift;
                best_cell = cell.cell;
                std::cout << "new cell:" << best_cell << std::endl;
            }
        }
        if (best_cell == NULL)
        {
            orientation = random_orientation();
            return find_move(grid, current_block);
        }
        else
        {
            return best_cell;
        }
    }
    else // back to the nest
    {
        return NULL;
    }

    // nothing ? let's get another orientation and retry
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
