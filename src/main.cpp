#include "Game.h"
#include <iostream>

int main(int argc, char **argv)
{
    auto game = game::Game();
    std::cout << "Running Program: " << argv[0] << std::endl;
    game.run();
}
