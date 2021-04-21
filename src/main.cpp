#include "controllers/ant.hpp"
#include <vector>

//todo: remove
std::vector<Ant> create_ants_array(const std::vector<Coordinates> &coordinates)
{
    std::vector<Ant> output;
    output.reserve(coordinates.size());
    for (size_t i = 0; i < coordinates.size(); i++)
        output.emplace_back(coordinates[i], i);
    return output;
}

int main()
{
    create_ants_array({Coordinates(1, -2)});
    return 0;
}