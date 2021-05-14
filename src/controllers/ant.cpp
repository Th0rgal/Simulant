#include "controllers/ant.hpp"
#include "controllers/random.hpp"
#include "models/grid.hpp"
#include "math.h"

double random_orientation()
{
    return random_double(0, 2 * M_PI);
}

Ant::Ant(Colony *colony, Coordinates coordinates) : location(coordinates),
                                                    sugar(false),
                                                    colony(colony),
                                                    orientation(random_orientation()),
                                                    origin_x(colony->centroid_x),
                                                    origin_y(colony->centroid_y)

{
}

Coordinates Ant::get_location()
{
    return location;
}

void Ant::set_location(const Coordinates &new_location)
{
    location = new_location;
}

Colony *Ant::get_colony()
{
    return colony;
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
    Cell *nest_oriented = NULL;

    bool pheromones = false;
    for (const Coordinates &coo : location.get_neighbors())
    {
        Cell *cell = grid.get_cell(coo, current_block);
        if (cell->has_sugar())
        {
            if (!has_sugar())
                sugar = cell;
            else
                continue;
        }
        if (cell->has_ant() && cell->get_ant()->colony == colony)
            continue;

        double angle = atan2(coo.x - origin_x, coo.y - origin_y);

        cells.push_back({cell,
                         coo.square_distance_to(origin_x, origin_y),
                         (angle < 0 ? angle + 2 * M_PI : angle)});

        if (cell->get_sugar_pheromones() > 0)
            pheromones = true;

        if (nest_oriented == NULL || cell->get_nest_pheromones(colony) > nest_oriented->get_nest_pheromones(colony))
            nest_oriented = cell;
    }

    if (!has_sugar()) // looking for sugar
    {
        // 1 we found sugar
        if (sugar != NULL)
            return sugar;

        // 2: we follow pheromones
        double current_distance = location.square_distance_to(origin_x, origin_y);
        std::vector<MetaCell> distancing;
        if (pheromones)
        {
            Cell *farthest = NULL;
            for (MetaCell meta_cell : cells)
                if (meta_cell.square_distance > current_distance)
                {
                    if (meta_cell.cell->get_sugar_pheromones() > (farthest == NULL ? 0 : farthest->get_sugar_pheromones()))
                        farthest = meta_cell.cell;
                }
            if (farthest != NULL)
                return farthest;
        }
        else
            for (MetaCell meta_cell : cells)
                if (meta_cell.square_distance > current_distance)
                    distancing.push_back(meta_cell);

        // 3: we explore the world
        if (cells.empty())
            return NULL;

        double best_orientation = M_PI; // magic value
        MetaCell *best_cell = NULL;
        for (MetaCell &cell : distancing)
        {
            double orientation_shift = abs(cell.orientation - orientation);
            if (orientation_shift > M_PI)
                orientation_shift = 2 * M_PI - orientation_shift;

            if (orientation_shift < best_orientation)
            {
                best_orientation = orientation_shift;
                best_cell = &cell;
            }
        }
        if (best_cell == NULL)
        {
            origin_x = location.x;
            origin_y = location.y;
            orientation = random_orientation();
            return find_move(grid, current_block);
        }
        else
        {
            Coordinates coo = best_cell->cell->get_location();
            if (coo.x >= X_MAX || coo.x <= X_MAX || coo.y >= Y_MAX || coo.y <= Y_MIN)
                orientation = random_orientation();
            return best_cell->cell;
        }
    }
    else // back to the nest
        return nest_oriented;
}

std::array<Cell *, 4> Colony::get_cells()
{
    return cells;
}
