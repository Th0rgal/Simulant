#include "models/grid.hpp"
#include "controllers/random.hpp"

/*
std::unordered_set<Coordinates> Grid::get_neighbors(const Coordinates &c) const
{
    std::unordered_set<Coordinates> neighbors;

    int x_min = std::max(c.get_x() - 1, X_MIN);
    int y_min = std::max(c.get_y() - 1, Y_MIN);
    int x_max = std::min(c.get_x() + 1, X_MAX + 1);
    int y_max = std::min(c.get_y() + 1, Y_MAX + 1);
    for (int y = y_min; y < y_max; y++) {
        for (int x = x_min; x < x_max; x++) {
            neighbors.emplace();
        }
    }
}*/

Cell::Cell(Coordinates location) : location(location)
{
}

// // find a void top left cell of a nest square (2x2 cell)
// std::array<Cell, 4> Grid::find_nest_cells()
// {
//     int x = random_index(X_MIN, X_MIN + SPACE_WIDTH / 2) * 2;
//     int y = random_index(X_MIN, X_MIN + SPACE_WIDTH / 2) * 2;
//     std::array<Cell, 4> output = {get_cell(x, y), get_cell(x + 1, y), get_cell(x, y + 1), get_cell(x + 1, y + 1)};
//     for (Cell &cell : output)
//         if (cell.is_void())
//             return find_nest_cells();
//     return output;
// };

Grid::Grid(size_t colonies_amount)
{
    for (int i = 0; i < SPACE_WIDTH * SPACE_HEIGHT; i++)
        map[i] = new Cell(Coordinates(X_MIN + i % SPACE_WIDTH, Y_MIN + i / SPACE_WIDTH));

    for (colonies_amount++; colonies_amount > 1; colonies_amount--)
    {
        //colonies.emplace_back(find_nest_cells());
        Colony &colony = colonies[colonies.size() - 1];
        // for (Cell &cell : colony.cells)
        // {
        //     cell.set_nest(colony);
        // }
    }
}
