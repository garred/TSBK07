#ifndef _interface_cpp_
#define _interface_cpp_

#include "interface.hpp"
#include "MicroGlut.h"
#include "entities.hpp"
#include "game.hpp"


Vehicle *Interface::player;

void Interface::init_all() {
    glutPassiveMotionFunc(Interface::mouse_update);
    player = NULL;
}


void Interface::draw_all() {
    //TODO
}


void Interface::keyboard_update() {
    float v = (glutKeyIsDown('q')) ? CAMERA_FAST : CAMERA_SLOW;

    /*
    // Camera manual movement legacy
    Camera::current->movement.x = (glutKeyIsDown('d') - glutKeyIsDown('a')) * v * Game::delta;
    Camera::current->movement.z = (glutKeyIsDown('w') - glutKeyIsDown('s')) * v * Game::delta;
    Camera::current->movement.y = (glutKeyIsDown('r') - glutKeyIsDown('f')) * v * Game::delta;
     */

    // Move the tank's body

    player->movement = SetVector(0,0,0);
    double move[2];
    move[1] = (glutKeyIsDown('w') - glutKeyIsDown('s')) * Game::delta;
    player->movement = (Ry(M_PI * 0.5) * player->direction * move[1] - player->direction * move[0]) * 10.0;

    double body_rotation = (glutKeyIsDown('a') - glutKeyIsDown('d')) * v * Game::delta * 0.01;
    player->direction = Ry(body_rotation) * player->direction;

    // Move the turret and cannon

    Entity*& turret = player->children.front();
    Entity*& cannon = player->children.front()->children.front();
    double turret_rotation = (glutKeyIsDown('h') - glutKeyIsDown('k')) * v * Game::delta * 0.01;
    turret->rotation += SetVector(0, turret_rotation, 0);
    double cannon_rotation = (glutKeyIsDown('j') - glutKeyIsDown('u')) * v * Game::delta * 0.01;
    cannon->rotation += SetVector(cannon_rotation, 0, 0);

    // Shoot

    if (glutKeyIsDown(' ')) {
        if (not player->trigger)
            Bullet::shoot(cannon);
        player->trigger = true;
    } else {
        player->trigger = false;
    }
}


void Interface::mouse_update(int x, int y) {

    /*
    // Camera mouse rotation legacy
    Camera::current->rotation.x = (x - 200) * 0.005;
    Camera::current->rotation.y = (y - 200) * 0.005;
     */
}


#endif
