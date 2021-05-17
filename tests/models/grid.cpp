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
    }
}
