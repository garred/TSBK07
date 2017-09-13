#ifndef _entity_cpp_
#define _entity_cpp_


#include "entity.hpp"
#include "graphics.hpp"
#include "helpers.hpp"
#include "entities.hpp"



Entity* Entity::world;


Entity::Entity() {
  this->parent = NULL;
  this->model = NULL;
  this->position = SetVector(0,0,0);
  this->rotation = SetVector(0,0,0);
  this->scale = SetVector(1,1,1);

  for (int i=0; i<8; i++) {
    this->texture_color[i] = (i==0) ? Graphics::white_texture : 0;
    this->texture_alpha[i] = (i==0) ? Graphics::white_texture : 0;
    this->texture_tile[i] = 1.0;
  }

  this->reflectivity = 1.0;
  this->shininess = 1.0;
  this->alpha = 1.0;
}


Entity::~Entity() {
  for (auto e : children)
    delete e;
  children.clear();
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


void Entity::update() {
  for (auto e : this->children)
    e->update();
}


void Entity::draw() {
  // First draws this entity, then draws its children
  if (model) {
    transformation_matrix = transformation(position, rotation, scale);
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

    Graphics::set_material(shininess, alpha, reflectivity);
    DrawModel(model, Graphics::shader_program, "inPosition", "inNormal", "inTexCoord");
  }

  // Draws its children
  for (auto e : children)
    e->draw();
}


void Entity::add_child(Entity* e) {
  children.push_back(e);
  e->parent = this;
}

#endif
