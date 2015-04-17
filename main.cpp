#include "Chess2D.h"
#include <ctime>

int main(int argc, char **argv)
{
    if(argc==1)
    {
        Chess2D game;
        game.StartGame();
    }
    else if(argc==2)
    {
        cout<<argv[1]<<endl;
        Chess2D game;
        game.StartGame(string(argv[1]));
    }
}
