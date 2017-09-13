// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-2.frag", "lab1-2.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"


// Consts & Globals

// Frustrum
float near = 0.1;
float far = 10000.0;
float right = 0.05;
float left = -0.05;
float top = 0.05;
float bottom = -0.05;

GLuint program; //Reference to shader program
mat4 projectionMatrix;

GLfloat t; //Time

// Objects management.
#include "src/helpers.c"
#include "src/material.c"
#include "src/camera.c"
#include "src/lights.c"
#include "src/static.cxx"
#include "src/skybox.c"
#include "src/terrain.c"
#include "src/windmill.c"



// Initialize everything

void init(void) {
	dumpInfo();

	// GL inits
	int texture_units;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
	printf("Number of textures: %i\n", texture_units);
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glFrontFace(GL_CCW);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("src/lab.vert", "src/lab.frag");
	printError("init shader");

	// Initialize objects of the world
	init_camera();
	init_lights();
	load_terrain();
	load_windmill();
	load_static_objects();
	load_skybox();


	// Set and send projection matrix to the vertex shader
	projectionMatrix = frustum(left,right, bottom,top, near,far);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	printError("init arrays");
}


// Display everything

void display(void) {
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Time info for shaders
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)*0.001;
	glUniform1f(glGetUniformLocation(program, "time"), t);

	// Setting the world
	update_camera();
	set_lights();
	draw_skybox();
	draw_windmill();
	move_tree();
	draw_static_objects();
	draw_terrain();

	// Flip
	glutSwapBuffers();
}


// Updates 50 frames per second

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
		windmill.blade_angle += 0.05;
		keyboard_func();
}


// Main

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(800,800);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	glutTimerFunc(20, &OnTimer, 0);
	glutPassiveMotionFunc(&mouse_func);

	init ();
	glutMainLoop();
	return 0;
}
