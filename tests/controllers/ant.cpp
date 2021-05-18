#include <doctest/doctest.h>
#include "controllers/ant.hpp"
#include "models/grid.hpp"

TEST_CASE("testing Colony getters and setters")
{
    Grid grid{1};
    size_t i = 0;
    grid.map_colony([&](Colony *colony) {
        i++;
        CHECK(colony->get_cells().size() == 4);
        CHECK_FALSE(colony->add_sugar());
        CHECK_FALSE(colony->add_sugar());
        CHECK(colony->add_sugar());
        colony->add_ant(NULL);
        size_t j = colony->find_ant_index(NULL);
        colony->remove_ant(grid, 0);
        CHECK(colony->find_ant_index(NULL) == j - 1);
    });
    CHECK(i == 1);
}
