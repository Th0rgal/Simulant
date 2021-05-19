#pragma once
#include <stdlib.h>

class GameOptions
{

public:
    GameOptions(int args_size, char *args[]);

    bool fullscreen_view = false;
    size_t colonies_amount = 3;
};