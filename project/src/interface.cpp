#ifndef _interface_cpp_
#define _interface_cpp_

#include "interface.hpp"
#include "MicroGlut.h"
#include "entities.hpp"
#include "game.hpp"


Node *Interface::player;

void Interface::init_all() {
    glutPassiveMotionFunc(Interface::mouse_update);
    player = NULL;
}


void Interface::draw_all() {
    //TODO
}


void Interface::keyboard_update() {
    float v = (glutKeyIsDown('q')) ? CAMERA_FAST : CAMERA_SLOW;
    Camera::current->movement.x = (glutKeyIsDown('d') - glutKeyIsDown('a')) * v * Game::delta;
    Camera::current->movement.z = (glutKeyIsDown('w') - glutKeyIsDown('s')) * v * Game::delta;
    Camera::current->movement.y = (glutKeyIsDown('r') - glutKeyIsDown('f')) * v * Game::delta;

//    player->velocity.x += (glutKeyIsDown('h') - glutKeyIsDown('k')) * 10 * Game::delta;
//    player->velocity.z += (glutKeyIsDown('u') - glutKeyIsDown('j')) * 10 * Game::delta;

    double move[2];
    move[0] = (glutKeyIsDown('h') - glutKeyIsDown('k')) * 10 * Game::delta;
    move[1] = (glutKeyIsDown('u') - glutKeyIsDown('j')) * 10 * Game::delta;
    player->velocity += Ry(M_PI*0.5)*player->direction * move[1] - player->direction * move[0];


    double rotation = (glutKeyIsDown('i') - glutKeyIsDown('y')) * v * Game::delta * 0.01;
    player->direction = Ry(-rotation) * player->direction;
    //player->velocity.y +=  (glutKeyIsDown('w') - glutKeyIsDown('s')) * v * Game::delta;

}


void Interface::mouse_update(int x, int y) {
    Camera::current->rotation.x = (x - 200) * 0.005;
    Camera::current->rotation.y = (y - 200) * 0.005;
}


#endif
