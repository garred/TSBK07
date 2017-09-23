#ifndef _entity_cpp_
#define _entity_cpp_


#include <algorithm>

#include "entity.hpp"
#include "graphics.hpp"
#include "helpers.hpp"
#include "entities.hpp"



Entity* Entity::world;


Entity::Entity() {
    parent = NULL;
    model = NULL;
    position = SetVector(0,0,0);
    rotation = SetVector(0,0,0);
    scale = SetVector(1,1,1);
    transformation_matrix = IdentityMatrix();
    alive = true;

    for (int i=0; i<8; i++) {
        texture_color[i] = (i==0) ? Graphics::white_texture : 0;
        texture_alpha[i] = (i==0) ? Graphics::white_texture : 0;
        texture_tile[i] = 1.0;
    }

    reflectivity = 1.0;
    shininess = 1.0;
    alpha = 1.0;
    use_light = true;
}


Entity::~Entity() {
}


void Entity::init_all() {
  // Creates the world (the root of the tree)
  Entity::world = new Entity();

  // Creates the default (and probably the only) camera and adds it to the world
  Camera::current = new Camera;
  Entity::world->add_child(Camera::current);
  Camera::current->position = SetVector(0,100,-100);
}


void Entity::update_all() {
  Entity::world->update();
}


void Entity::draw_all() {
  Entity::world->draw();
}

void Entity::remove_all_died() {
    Entity::world->remove_died();
}


void Entity::update() {
  for (auto e : this->children)
    e->update();
}


void Entity::draw() {
    // First draws this entity, then draws its children
    if (model) {
        transformation_matrix = get_global_matrix();
        glUniformMatrix4fv(glGetUniformLocation(Graphics::shader_program, "transformationMatrix"), 1, GL_TRUE, transformation_matrix.m);

        GLint colorTexUnit[8], alphaTexUnit[8];
        GLfloat tilesTexUnit[8];
        for (int i=0; i<8; i++) {
            glActiveTexture(GL_TEXTURE0+i);
            glBindTexture(GL_TEXTURE_2D, texture_color[i]);
            glActiveTexture(GL_TEXTURE0+i+8);
            glBindTexture(GL_TEXTURE_2D, texture_alpha[i]);
            colorTexUnit[i] = i;
            alphaTexUnit[i] = 8+i;
            tilesTexUnit[i] = texture_tile[i];
        }
        glUniform1iv(glGetUniformLocation(Graphics::shader_program, "colorTexUnit"), 8, colorTexUnit);
        glUniform1iv(glGetUniformLocation(Graphics::shader_program, "alphaTexUnit"), 8, alphaTexUnit);
        glUniform1fv(glGetUniformLocation(Graphics::shader_program, "tilesTexUnit"), 8, tilesTexUnit);

        Graphics::set_material(shininess, alpha, reflectivity, use_light);
        DrawModel(model, Graphics::shader_program, "inPosition", "inNormal", "inTexCoord");
    }

    // Draws its children
    for (auto e : children)
        e->draw();
}

void Entity::remove_died() {
    for (auto e : children) {
        e->alive *= alive;
        e->remove_died();
    }

    children.remove_if([](Entity* e) {
        return not e->alive;
    });

    if (not alive) delete this;
}


void Entity::add_child(Entity* e) {
  children.push_back(e);
  e->parent = this;
}


mat4 Entity::get_transformation_matrix() {
    return transformation(position, rotation, scale);
}

mat4 Entity::get_global_matrix() {
    mat4 m = IdentityMatrix();
    Entity* e = this;

    while (e) {
        m = e->get_transformation_matrix() * m;
        e = e->parent;
    }

    return m;
}

#endif
