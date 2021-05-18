#include <doctest/doctest.h>
#include "controllers/random.hpp"

TEST_CASE("testing random_index")
{
    for (size_t i = 0; i < 100; i++)
    {
        int value = random_index(-100, 100);
        bool correct = (value >= -100) && (value <= 100);
        CHECK(correct);
    }
}

TEST_CASE("testing random_double")
{
    for (size_t i = 0; i < 100; i++)
    {
        double value = random_double(-1, 1);
        bool correct = (value >= -1) && (value <= 1);
        CHECK(correct);
    }
}