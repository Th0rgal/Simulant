#include <random>

std::mt19937 gen(std::random_device{}());

int random_index(int lower, int higher)
{
    return std::uniform_int_distribution<int>(lower, higher)(gen);
}
