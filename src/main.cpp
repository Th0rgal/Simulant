#include "controllers/game.hpp"
#include "controllers/cli.hpp"

int main(int args_size, char *args[])
{
    Game(GameOptions(args_size, args)).start();
    return 0;
}