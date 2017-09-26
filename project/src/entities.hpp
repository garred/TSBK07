#ifndef _entities_hpp_
#define _entities_hpp_

#include "entity.hpp"
#include "LoadTGA.h"
#include "VectorUtils3.h"
#include "random.hpp"
#include <tuple>
#include <iostream>


/*******************************************************************************
********************************************************************************
*******************************************************************************/

class Vehicle;

class Camera : public Entity {
public:
    static Camera *current;

    vec3 l, v;
    mat4 lookAtMatrix;
    vec3 movement;

    Entity* following;      //If the camera is following a vehicle
    vec3 relative_position; //distance to follow
    vec3 relative_look_at;  //pos to look
    double following_velocity;

    void update();

    Camera() {
        position = SetVector(0, 1, -3);
        l = SetVector(0, 0, 0);
        v = SetVector(0, 1, 0);
        movement = SetVector(0, 0, 0);
        rotation = SetVector(0, 0, 0);

        following = nullptr;
        relative_position = SetVector(0,3,-10);
        relative_look_at = SetVector(0,0,10);
        following_velocity = 0.05;
    }

    ~Camera() {}
};


/*******************************************************************************
********************************************************************************
*******************************************************************************/

class Box : public Entity {
public:

    Box() {
        this->model = LoadModelPlus("media/skybox.obj");
        LoadTGATextureSimple("media/SkyBox512.tga", &(this->texture_color[0]));
        scale = 1000*SetVector(-1,1,1);
        use_light = false;
        //this->model = LoadModelPlus("media/teapot.obj");
        //LoadTGATextureSimple("media/grass.tga", &(this->texture_color[0]));
    }

    void update() {
        Entity::update();
        position = Camera::current->position;
    }

    ~Box() {}
};


/*******************************************************************************
********************************************************************************
*******************************************************************************/


class Terrain : public Entity {
public:
    static Terrain *current;

    int width, height;

    void load_terrain();

    void randomize();

    float get_height(vec3 object_position);

    vec3 get_normal(vec3 object_position);
};


/*******************************************************************************
********************************************************************************
*******************************************************************************/


class Vehicle : public Entity {
public:
    static Model *body_model, *turret_model, *cannon_model;
    static GLuint model_texture;

    vec3 velocity;
    vec3 movement;
    vec3 ground;
    vec3 normal;
    vec3 normal_vel;

    float bounceness;
    float slideness;

    vec3 direction;

    bool trigger;


    Vehicle() {
        bounceness = 0.0;
        slideness = 0.9;

        // Loading the models
        model = body_model;
        texture_color[0] = model_texture;

        Entity* turret = new Entity();
        turret->model = turret_model;
        turret->texture_color[0] = texture_color[0];
        turret->position = SetVector(0.0, 1.37527*1.666, -1.2696);
        add_child(turret);

        Entity* cannon = new Entity();
        cannon->model = cannon_model;
        cannon->texture_color[0] = texture_color[0];
        turret->add_child(cannon);

        // Positioning the object
        scale = SetVector(10, 10, 10);
        velocity = SetVector(0,0,0);
        movement = SetVector(0,0,0);
        ground = SetVector(0,0,0);
        normal = SetVector(0,1,0);
        normal_vel = SetVector(0,0,0);

        direction = SetVector(0, 0, 1);
    }

    void update();

    mat4 get_transformation_matrix() override ;

private:

    mat4 _get_terrain_orientation();
    std::tuple<vec3,vec3,vec3,vec3> _update_ground();
};


/*******************************************************************************
********************************************************************************
*******************************************************************************/


class Bullet : public Entity {
public:
    static Model* bullet_model;

    vec3 velocity;

    static void shoot(Entity* from) {
        Bullet* b = new Bullet();
        b->model = bullet_model;
        auto m = from->get_global_matrix();
        b->position = m * (from->position + SetVector(0,0,3));
        b->velocity = m * (from->position + SetVector(0,0,100)) - b->position;
        Entity::world->add_child(b);
    }

    void update();
};

/*******************************************************************************
********************************************************************************
*******************************************************************************/


class Explosion : public Entity {
public:
    static Model* explosion_model;
    static GLuint fire_texture;

    int life;

    static void create(vec3 pos) {
        Explosion* e = new Explosion();
        e->model = explosion_model;
        e->texture_color[0] = fire_texture;
        e->position = pos;
        e->scale *= -1;
        e->rotation = SetVector(Random::real(-10,10),Random::real(-10,10),Random::real(-10,10));

        e->destroy_close_objects();

        Entity::world->add_child(e);
        e->life = 100;
        e->alpha = 0.5;
        e->use_light = false;
    }

    void update() override;

private:
    void destroy_close_objects();
};



/*******************************************************************************
********************************************************************************
*******************************************************************************/


class Enemy : public Vehicle {
public:
    void update() override;
};



/*******************************************************************************
********************************************************************************
*******************************************************************************/

class Tree : public Entity {
public:

    static Model* tree_model;
    static GLuint tree_texture;

    Tree() {
        model = tree_model;
        texture_color[0] = tree_texture;
        rotation.y = Random::real(-100,100);
        scale = SetVector(1,1,1) * Random::real(15,30);
    }

    ~Tree() {}
};




#endif