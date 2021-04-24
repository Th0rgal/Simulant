#include "models/grid.hpp"
#include "controllers/random.hpp"
#include "algorithm"
#include "math.h"

Cell::Cell(Coordinates location) : location(location)
{
}

bool Cell::is_void()
{
    return !sugar && !is_nest() && !has_ant();
}

Coordinates Cell::get_location() const
{
    return location;
}

void Cell::set_nest(Colony *colony)
{
    nest = colony;
}

bool Cell::is_nest()
{
    return nest != NULL;
}

Colony *Cell::get_nest()
{
    return nest;
}

bool Cell::has_ant()
{
    return ant != NULL;
}

void Cell::set_ant(Ant *new_ant)
{
    ant = new_ant;
}

void Cell::remove_ant()
{
    delete ant;
    ant = NULL;
}

Ant *Cell::get_ant()
{
    return ant;
}

double Cell::get_nest_pheromons(const Colony *colony)
{
    if (nest_pheromons.find(colony) == nest_pheromons.end())
        return 0;
    return nest_pheromons[colony];
}

// find a void top left cell of a nest square (2x2 cell)
std::array<Cell *, 4> Grid::find_nest_cells()
{
    int x = (random_index(X_MIN, 0) + SPACE_WIDTH / 4) * 2;
    int y = (random_index(Y_MIN, 0) + SPACE_HEIGHT / 4) * 2;
    std::array<Cell *, 4> output = {get_cell(x, y), get_cell(x + 1, y), get_cell(x, y + 1), get_cell(x + 1, y + 1)};
    for (Cell *cell : output)
        if (cell == NULL || cell->is_nest())
            return find_nest_cells();
    return output;
};

Grid::Grid(size_t colonies_amount)
{
    for (int i = 0; i < SPACE_WIDTH * SPACE_HEIGHT; i++)
        map[i] = new Cell(Coordinates(X_MIN + i % SPACE_WIDTH, Y_MIN + i / SPACE_WIDTH));

    for (colonies_amount++; colonies_amount > 1; colonies_amount--)
    {
        Colony *colony = new Colony(find_nest_cells());
        colonies.push_back(colony);
        double centroid_x = 0;
        double centroid_y = 0;
        for (Cell *cell : colony->get_cells())
        {
            centroid_x += cell->get_location().x;
            centroid_y += cell->get_location().y;
            if (cell->has_ant())
                cell->remove_ant();

            cell->set_nest(colony);
            cell->nest_pheromons[colony] = 1;
        }
        centroid_x /= colony->get_cells().size();
        centroid_y /= colony->get_cells().size();
        double max_square_distance = (SPACE_WIDTH - 0.5) * (SPACE_WIDTH - 0.5) + (SPACE_HEIGHT - 0.5) * (SPACE_HEIGHT - 0.5);
        for (Cell *cell : map)
            if (cell->get_nest_pheromons(colony) != 1)
                cell->nest_pheromons[colony] = 1 - cell->get_location().distance_to(centroid_x, centroid_y) / std::sqrt(max_square_distance);
        //get_cell(centroid_x - 0.5, centroid_y - 0.5)->set_ant(new Ant(colony, Coordinates(centroid_x -0.5, centroid_y - 0.5)));
        //cell->set_ant(new Ant(colony, Coordinates(cell->get_location())));
        spawn_ants(colony, centroid_x - 1.5, centroid_y - 1.5);
    }
}

void Grid::spawn_ants(Colony *colony, int x, int y)
{
    auto fetch_cell = [&](size_t perm, int x, int y, int i) {
        switch (perm)
        {
        case 0:
            return get_cell(x + i, y);
        case 1:
            return get_cell(x + 3, y + i);
        case 2:
            return get_cell(x + i, y + 3);
        default:
            return get_cell(x, y + i);
        }
    };
    Cell *cell = get_cell(x, y);
    if (cell != NULL && !cell->is_nest() && (!cell->has_ant() || flip_a_coin()))
        cell->set_ant(new Ant(colony, cell->get_location()));
    for (size_t permutation = 0; permutation <= 3; permutation++)
        for (int i = 1; i < 4; i++)
        {
            Cell *cell = fetch_cell(permutation, x, y, i);
            if (cell == NULL || cell->is_nest() || (cell->has_ant() && flip_a_coin())) // to keep nest conflicts fair
                continue;
            cell->set_ant(new Ant(colony, cell->get_location()));
        }
}

Cell *Grid::get_cell(int x, int y)
{
    return (x >= X_MIN && x <= X_MAX && y >= Y_MIN && y <= Y_MAX) ? map[(y - Y_MIN) * SPACE_WIDTH + x - X_MIN] : NULL;
}

Cell *Grid::get_cell(Coordinates location)
{
    return map[(location.y - Y_MIN) * SPACE_WIDTH + location.x - X_MIN];
}