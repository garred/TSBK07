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
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"


// Consts & Globals

// Frustrum
#define near 1.0
#define far 100.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

GLuint program; //Reference to shader program
GLuint vertexArrayObjID; //Vertex array object
GLuint texture; //Loaded texture
mat4 projectionMatrix;

// Windmill struct and functions

typedef struct {
	Model *blade, *walls, *roof, *balcony;
	float blade_angle;
	vec3 pos, rot, sca;
	mat4 blade_transform[4];
} Windmill;

Windmill windmill;

void load_windmill(Windmill* w) {
	w->blade = LoadModelPlus("windmill/blade.obj");
	w->walls = LoadModelPlus("windmill/windmill-walls.obj");
	w->roof = LoadModelPlus("windmill/windmill-roof.obj");
	w->balcony = LoadModelPlus("windmill/windmill-balcony.obj");
	w->blade_angle = 0;
	w->pos = SetVector(0,0,0);
	w->rot = SetVector(0,0,0);
	w->sca = SetVector(0,0,0);
	// Pre-calculating the position of each blade
	for (int i=0; i<4; i++) {
		w->blade_transform[i] = Mult(T(4.5,9.25,0), Rx(M_PI*i*0.5));
	}
}

void draw_windmill(Windmill* w) {
	mat4 transformationMatrix;

// THINGS TO DO:
// - Store the transformation matrix of each blade in the object.
// - Calculate a rotation matrix and use it for every blade.

	// Drawing the blades
	mat4 blade_rotation = Rx(w->blade_angle);
	for (int i=0; i<4; i++) {
		transformationMatrix = Mult(w->blade_transform[i], blade_rotation);
		glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
		DrawModel(w->blade, program, "inPosition", "inNormal", "inTexCoord");
	}

	// Drawing the body
	transformationMatrix = T(0,0,0);
	glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
	DrawModel(w->walls, program, "inPosition", "inNormal", "inTexCoord");
	DrawModel(w->roof, program, "inPosition", "inNormal", "inTexCoord");
	DrawModel(w->balcony, program, "inPosition", "inNormal", "inTexCoord");
}


// Camera struct and functions

typedef struct {
	vec3 p, l, v;
} Camera;

Camera camera;

void init_camera(Camera* c) {
	c->p = SetVector(0,1,-3);
	c->l = SetVector(0,0,0);
	c->v = SetVector(0,0,0);
}

void update_camera(Camera *c, float t) {
	c->p = SetVector(sin(t)*25,15,cos(t)*25);
	c->l = SetVector(0,7,0);
	c->v = SetVector(0,1,0);
	mat4 lookAtMatrix = lookAtv(c->p,c->l,c->v);
	glUniformMatrix4fv(glGetUniformLocation(program, "lookAtMatrix"), 1, GL_TRUE, lookAtMatrix.m);
}


// Initialize everything

void init(void) {
	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
	printError("init shader");

	// Load the windmill
	load_windmill(&windmill);

	// Initialize the camera
	init_camera(&camera);

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
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)*0.001;
	glUniform1f(glGetUniformLocation(program, "time"), t);

	// Setting the world
	update_camera(&camera, t);
	draw_windmill(&windmill);

	// Flip
	glutSwapBuffers();
}


// Updates 50 frames per second

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
		windmill.blade_angle += 0.05;
}


// Main

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	glutTimerFunc(20, &OnTimer, 0);

	init ();
	glutMainLoop();
	return 0;
}
