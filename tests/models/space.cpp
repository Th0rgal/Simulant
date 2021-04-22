#include <doctest/doctest.h>
#include "models/space.hpp"

TEST_CASE("testing vectors")
{

    Vector alpha{1, 2};
    Vector beta{-3, 4};
    Vector gamma{3, -5};
    Vector delta{-8, -1};
    Vector epsilon{-1, 0};

    CHECK(alpha + beta == Vector{-2, 6});
    CHECK(alpha - beta == Vector{4, -2});
    CHECK(beta - gamma == Vector{-6, 9});

    delta += epsilon;
    CHECK_FALSE(delta == delta + epsilon);
    CHECK(delta == Vector{-9, -1});

    CHECK(beta * 0 == Vector{0, 0});
    CHECK(epsilon * 2 == Vector{-2, 0});

    epsilon *= 2;
    CHECK(epsilon == Vector{-2, 0});

    CHECK(beta / 2 == Vector{-1, 2});
    beta /= 2;
    CHECK(beta == Vector{-1, 2});
}

TEST_CASE("testing coord") 
{
    Coordinates a(0, 0);
    Coordinates b(-1, 1);
    Coordinates c(1, -1);

    CHECK(a.get_x() == 0);
    CHECK(a.get_y() == 0);
    CHECK(b.get_x() == -1);
    CHECK(b.get_y() == 1);
    CHECK(c.get_x() == 1);
    CHECK(c.get_y() == -1);
    
    CHECK(X_MAX == 10);
    CHECK(Y_MAX == 10);
    CHECK(X_MIN == -10);
    CHECK(X_MIN == -10);


    CHECK(c == c);
    CHECK_FALSE(c == a);
    CHECK(c != a);
    CHECK_FALSE(c != c);
    
    //Coordinates d(-20, 0);

}