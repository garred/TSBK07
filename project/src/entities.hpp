#ifndef _entities_hpp_
#define _entities_hpp_

#include "entity.hpp"
#include "LoadTGA.h"
#include "VectorUtils3.h"


/*******************************************************************************
********************************************************************************
*******************************************************************************/

class Box : public Entity {
public:

    Box() {
        //this->model = LoadModelPlus("media/skybox.obj");
        //LoadTGATextureSimple("media/SkyBox512.tga", &(this->texture_color[0]));
        this->model = LoadModelPlus("media/teapot.obj");
        LoadTGATextureSimple("media/grass.tga", &(this->texture_color[0]));
    }

    ~Box() {}
};


/*******************************************************************************
********************************************************************************
*******************************************************************************/


class Camera : public Entity {
public:
    static Camera *current;

    vec3 l, v;
    mat4 lookAtMatrix;
    vec3 movement;

    void update();

    Camera() {
        this->position = SetVector(0, 1, -3);
        this->l = SetVector(0, 0, 0);
        this->v = SetVector(0, 1, 0);
        this->movement = SetVector(0, 0, 0);
        this->rotation = SetVector(0, 0, 0);
    }

    ~Camera() {}
};


/*******************************************************************************
********************************************************************************
*******************************************************************************/


class Terrain : public Entity {
public:
    static Terrain *current;

    int width, height;

    void load_terrain();

    float get_height(vec3 object_position);

    vec3 get_normal(vec3 object_position);
};


/*******************************************************************************
********************************************************************************
*******************************************************************************/


class Node : public Entity {
public:
    static void draw_all();

    float size;
    vec3 velocity;
    vec3 spring_velocity;
    float bounceness = 0.0;
    float slideness = 0.1;

    vec3 direction; //TODO


    Node() {
        model = LoadModelPlus("media/cube.obj");
        size = 1.0;
        scale = SetVector(10, 10, 10);

        direction = SetVector(0, 0, 1); //TODO
    }

    void update();

    void draw();
};


#endif
