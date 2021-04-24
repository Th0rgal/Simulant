#include "models/grid.hpp"
#include "controllers/random.hpp"

Cell::Cell(Coordinates location) : location(location)
{
}

Coordinates Cell::get_location() const {
    return location;
}

void Cell::set_nest(Colony &colony) {

}

bool Cell::is_void()
{
    return !sugar && sugar_pheromon == 0 && nest_pheromon == 0 && ant == NULL;
}

// find a void top left cell of a nest square (2x2 cell)
std::array<Cell *, 4> Grid::find_nest_cells()
{
    int x = (random_index(X_MIN, 0) + SPACE_WIDTH/4) * 2;
    int y = (random_index(Y_MIN, 0) + SPACE_HEIGHT/4) * 2;
    std::array<Cell *, 4> output = {get_cell(x, y), get_cell(x + 1, y), get_cell(x, y + 1), get_cell(x + 1, y + 1)};
    for (Cell *cell : output)
        if (cell == NULL)
            return find_nest_cells();
    return output;
};

Grid::Grid(size_t colonies_amount)
{
    for (int i = 0; i < SPACE_WIDTH * SPACE_HEIGHT; i++)
        map[i] = new Cell(Coordinates(X_MIN + i % SPACE_WIDTH, Y_MIN + i / SPACE_WIDTH));

    for (colonies_amount++; colonies_amount > 1; colonies_amount--)
    {
        colonies.emplace_back(find_nest_cells());
        Colony &colony = colonies[colonies.size() - 1];
        for (Cell *cell : colony.get_cells())
        {
            cell->set_nest(colony);
        }
    }
}

Cell *Grid::get_cell(int x, int y)
{
    return (x >= X_MIN && x <= X_MAX && y >= Y_MIN && y <= Y_MAX) ? map[(y - Y_MIN) * SPACE_WIDTH + x - X_MIN] : NULL;
}