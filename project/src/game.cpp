#ifndef _game_cpp_
#define _game_cpp_


#include "game.hpp"
#include "graphics.hpp"
#include "interface.hpp"
#include "entity.hpp"
#include "entities.hpp"
#include "random.hpp"


bool Game::is_running = true;
float Game::time = .0f;
float Game::delta = .0f;


void Game::init_all() {
    Graphics::init_all();

    Random::init_all();

    // Set the function that updates game logic and interface n times per second.
    glutTimerFunc(15, &Game::update_all, 0);

    // Set the function that redraws in glut's inner loop
    glutDisplayFunc(Game::draw_all);

    Interface::init_all();
    Entity::init_all();

    // Create a box in the middle
    Entity* my_box = new Box();
    Entity::world->add_child(my_box);

    // Creates a terrain
    Terrain::current = new Terrain();
    //Terrain::current->load_terrain();
    Terrain::current->randomize();
    Entity::world->add_child(Terrain::current);

    // Create a vehicle for the player
    Vehicle* my_node = new Vehicle();
    my_node->position = SetVector( 0, 1000, 0);
    Entity::world->add_child(my_node);
    Interface::player = my_node;

    // Sets the camera to follow the vehicle (specifically, the cannon)
    Camera::current->following = my_node->children.front()->children.front();

    // Create some enemies
    for (int i=0; i<50; i++) {
        Enemy* enemy = new Enemy();
        enemy->position = SetVector(Random::real(-1,1),0,Random::real(-1,1)) * 2000.0;
        Entity::world->add_child(enemy);
    }

    // Create some trees
    for (int i=0; i<200; i++) {
        Tree* tree = new Tree();
        bool placed = false;
        while(not placed) {
            tree->position = SetVector(Random::real(-1, 1), 0, Random::real(-1, 1)) * 3000.0;
            tree->position.y = Terrain::current->get_height(tree->position);
            placed = tree->position.y > 100 and tree->position.y < 1000;
        }
        Entity::world->add_child(tree);
    }

    time = (GLfloat)glutGet(GLUT_ELAPSED_TIME)*0.001;
}


void Game::draw_all() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Entity::draw_all();
  Interface::draw_all();          //TODO: Render the 2d GUI
  Graphics::draw_all();
}


void Game::update_all(int value) {
    glutTimerFunc(15, &Game::update_all, value);
    glutPostRedisplay();

    // Time info for shaders and game logic
    float t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)*0.001;
    glUniform1f(glGetUniformLocation(Graphics::shader_program, "time"), t);
    delta = t - time;
    time = t;

    Entity::update_all();
    Entity::remove_all_died();
    Interface::keyboard_update();
    Graphics::update_lights();
    //glutWarpPointer(200,200);       //Resets the mouse position
}

#endif
