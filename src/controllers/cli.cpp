#include "controllers/cli.hpp"
#include <iostream>
#include <cstring>

GameOptions::GameOptions(int args_size, char *args[])
{
    for (size_t i = 0; i < args_size; i++)
    {
        char *arg = args[i];
        if (!strcmp(arg, "nests") || !strcmp(arg, "--nests") || !strcmp(arg, "-n"))
        {
            colonies_amount = std::atoi(args[++i]);
            continue;
        }
        if (!strcmp(arg, "full") || !strcmp(arg, "--full") || !strcmp(arg, "-f"))
        {
            fullscreen_view = true;
            continue;
        }
        if (!strcmp(arg, "-fn"))
        {
            fullscreen_view = true;
            colonies_amount = std::atoi(args[++i]);
            break;
        }
    }
}