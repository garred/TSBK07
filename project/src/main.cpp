#include "game.hpp"
#include "MicroGlut.h"


int main(int argc, char **argv) {
    Game::init_all();
    glutMainLoop(); //TODO: Game::draw(), Game::update()
    return 0;
}
