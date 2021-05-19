#pragma once
#include <stdlib.h>

class GameOptions
{

public:
    /**
     * to intialize a GameOptions object
     * 
     * @param int amount of parameters
     * @param char* parameters
     * 
     * @confidence 5: no tests needed
     * @author Thomas Marchand
     **/
    GameOptions(int args_size, char *args[]);

    bool fullscreen_view = false;
    size_t colonies_amount = 3;
};