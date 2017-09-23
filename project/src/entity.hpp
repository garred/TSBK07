#ifndef _entity_hpp_
#define _entity_hpp_

#include <list>
#include "VectorUtils3.h"
#include "loadobj.h"

class Entity {
protected:
    mat4 transformation_matrix;
public:
    Model* model;
    vec3 position, rotation, scale;
    GLuint texture_color[8], texture_alpha[8];
    GLfloat texture_tile[8];
    GLfloat reflectivity, shininess, alpha;   //reflectivity is k_d
    bool use_light;
    std::list<Entity*> children;
    Entity* parent;
    bool alive;

    static Entity* world;   //Entities are organized as a tree. World is the root.

    static void init_all();
    static void update_all();
    static void draw_all();
    static void remove_all_died();

    Entity();
    virtual ~Entity();
    virtual void update();
    virtual void draw();
    void add_child(Entity* e);
    void remove_died();

    mat4 get_global_matrix();
    virtual mat4 get_transformation_matrix();
};


#endif
