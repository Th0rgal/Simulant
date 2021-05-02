#include "models/grid.hpp"
#include "controllers/random.hpp"
#include "algorithm"
#include "math.h"

Cell::Cell(Coordinates location) : location(location), sugar(false), ant(NULL), nest(NULL)
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

Ant *Cell::get_ant()
{
    return ant;
}

bool Cell::has_sugar()
{
    return sugar;
}

void Cell::add_sugar()
{
    sugar = true;
}

double Cell::get_nest_pheromons(const Colony *colony)
{
    if (nest_pheromons.find(colony) == nest_pheromons.end())
        return 0;
    return nest_pheromons[colony];
}

double Cell::get_sugar_pheromons()
{
    return sugar_pheromons;
}

void Cell::add_sugar_pheromon()
{
    sugar_pheromons += 1;
}

void Cell::reduce_sugar_pheromon()
{
    sugar_pheromons *= 0.9;
}

void Cell::update(size_t current_block) {
    sugar_pheromons *= std::pow(0.9, current_block-last_block);
}

// find a void top left cell of a nest square (2x2 cell)
std::array<Cell *, 4> Grid::find_nest_cells()
{
    int x = (random_index(X_MIN, 0) + SPACE_WIDTH / 4) * 2;
    int y = (random_index(Y_MIN, 0) + SPACE_HEIGHT / 4) * 2;
    std::array<Cell *, 4> output = {get_cell(x, y), get_cell(x + 1, y),
                                    get_cell(x, y + 1), get_cell(x + 1, y + 1)};
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
        double max_square_distance = (SPACE_WIDTH - 0.5) *
                                         (SPACE_WIDTH - 0.5) +
                                     (SPACE_HEIGHT - 0.5) * (SPACE_HEIGHT - 0.5);
        for (Cell *cell : map)
        {
            if (cell->get_nest_pheromons(colony) != 1)
            {
                cell->nest_pheromons[colony] =
                    1 - cell->get_location().distance_to(colony->centroid_x, colony->centroid_y) / std::sqrt(max_square_distance);
            }
        }
    }
    for (Colony *colony : colonies)
        summon_ants(colony);
    summon_sugars(colonies.size()*5);
}

void Grid::summon_ants(Colony *colony)
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
    int x = colony->left_corner_x - 1;
    int y = colony->left_corner_y - 1;
    Cell *cell = get_cell(x, y);
    if (cell != NULL && !cell->is_nest())
    {
        Ant *new_ant = new Ant(colony, cell->get_location());
        cell->set_ant(new_ant);
        colony->add_ant(new_ant);
    }
    for (size_t permutation = 0; permutation <= 3; permutation++)
        for (int i = 1; i < 4; i++)
        {
            Cell *cell = fetch_cell(permutation, x, y, i);
            if (cell == NULL || cell->is_nest() || cell->has_ant())
                continue;
            Ant *new_ant = new Ant(colony, cell->get_location());
            cell->set_ant(new_ant);
            colony->add_ant(new_ant);
        }
}

void Grid::summon_sugars(size_t amount)
{
    std::vector<Coordinates> sugar_locations;
    for (size_t i = 0; i < amount; i++)
    {
        double best_distance = -1;
        Cell *best_cell;
        for (size_t i = 0; i < 7; i++)
        {
            Cell *cell = find_empty_cell();
            double distance = 0;
            for (Colony *colony : colonies)
                distance += cell->location.square_distance_to(colony->centroid_x, colony->centroid_y);
            for (const Coordinates &sugar_location : sugar_locations)
                distance += cell->location.square_distance_to(sugar_location) * 1.5;
            if (distance > best_distance)
            {
                best_distance = distance;
                best_cell = cell;
            }
        }
        best_cell->add_sugar();
        sugar_locations.push_back(best_cell->get_location());
    }
}

Cell *Grid::find_empty_cell()
{
    Cell *cell = get_cell(random_index(X_MIN, X_MAX), random_index(Y_MIN, Y_MAX));
    return !cell->is_void() ? find_empty_cell() : cell;
}

Cell *Grid::get_cell(const Vector &vector) const
{
    return (vector.x >= X_MIN && vector.x <= X_MAX && vector.y >= Y_MIN && vector.y <= Y_MAX)
               ? map[(vector.y - Y_MIN) * SPACE_WIDTH + vector.x - X_MIN]
               : NULL;
}

Cell *Grid::get_cell(int x, int y) const
{
    return (x >= X_MIN && x <= X_MAX && y >= Y_MIN && y <= Y_MAX)
               ? map[(y - Y_MIN) * SPACE_WIDTH + x - X_MIN]
               : NULL;
}

Cell *Grid::get_cell(Coordinates location, size_t current_block) const
{
    Cell* cell = map[(location.y - Y_MIN) * SPACE_WIDTH + location.x - X_MIN];
    if (current_block != 0)
        cell->update(current_block);
    return cell;
}

void Grid::clear()
{
    for (Cell *cell : map)
        delete cell;
    for (Colony *colony : colonies)
        delete colony;
    colonies.clear();
}

void Grid::set_ant(Ant *ant, Coordinates coordinates)
{
    int pos = coordinates.x * coordinates.y * SPACE_WIDTH;
}

void Grid::spawn_ants(Colony *colony, size_t amount)
{

    for (size_t i = amount + 1; i > 1; i--)
    {
        Vector start = Vector{colony->left_corner_x, colony->left_corner_y};
        int side_length = 0;
        Cell *search;
        while (search == NULL || !search->is_void())
            for (int i = 0; i <= side_length; i++)
            {
                start -= Vector{1, 1};
                side_length += 2;
                search = get_cell(start + Vector{i, 0});
                if (search != NULL && search->is_void())
                    break;
                search = get_cell(start + Vector{side_length + 1, i});
                if (search != NULL && search->is_void())
                    break;
                search = get_cell(start + Vector{i, side_length + 1});
                if (search != NULL && search->is_void())
                    break;
                search = get_cell(start + Vector{0, i});
                if (search != NULL && search->is_void())
                    break;
            }

        Ant *new_ant = new Ant(colony, search->get_location());
        search->set_ant(new_ant);
        colony->add_ant(new_ant);
    }
}