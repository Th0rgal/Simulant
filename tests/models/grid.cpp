#include <doctest/doctest.h>
#include "models/grid.hpp"

TEST_CASE("testing Colony getters and setters")
{
    for (size_t i = 1; i < 10; i++)
    {
        Grid grid{i};
        size_t j = 0;
        grid.map_colony([&](Colony *colony) {
            j++;
        });
        CHECK(j == i);

        Cell *cell = grid.get_cell(Vector{0, 0});
        Cell *cell2 = grid.get_cell(0, 0);
        Cell *cell3 = grid.get_cell(Coordinates{0, 0}, 0);
        CHECK(cell == cell2);
        CHECK(cell == cell3);
    }
}
