#include <iostream>
using namespace std;

#include "fundamentals/Game.h"

int main(int argc, char *argv[])
{
    Game game;

    game.initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    //game loop
    while (game.isRunning())
    {
        game.processInput();
        game.update();
        game.render();
    }
    
    game.destroy();
return 0;
}