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

#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5


// Globals

Model *model1, *model2;	//Loaded model
GLuint program; //Reference to shader program
GLuint vertexArrayObjID; //Vertex array object
GLuint texture; //Loaded texture
mat4 projectionMatrix;


void init(void)
{

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	printError("GL inits");

	// Initializes the projection and lookAt matrices
	projectionMatrix = frustum(left,right, bottom,top, near,far);

	// Load and compile shader
	program = loadShaders("lab2-7.vert", "lab2-7.frag");
	printError("init shader");

	// Load two models
	model1 = LoadModelPlus("lab2models/bunnyplus.obj");
	model2 = LoadModelPlus("lab2models/teddy.obj");

	// Load texture
	LoadTGATextureSimple("lab2models/maskros512.tga", &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

	// Sending projection matrix to the vertex shader
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)*0.001;

	glUniform1f(glGetUniformLocation(program, "time"), t);


	mat4 rot, trans, transformationMatrix;

	// Moving the camera
	vec3 p, l, v;
	p = SetVector(sin(t)*3,1,cos(t)*3);
	l = SetVector(0,0,0);
	v = SetVector(0,1,0);
	mat4 lookAtMatrix = lookAtv(p,l,v);
	glUniformMatrix4fv(glGetUniformLocation(program, "lookAtMatrix"), 1, GL_TRUE, lookAtMatrix.m);

	// Drawing object 1
	rot = Mult(Ry(t*2), Rx(t));
	trans = T(1,0,0);
	transformationMatrix = Mult(trans, rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
	DrawModel(model1, program, "inPosition", "inNormal", "inTexCoord");

	// Drawing object 2
	trans = T(-1,0,0);
	transformationMatrix = trans;
	glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
	DrawModel(model2, program, "inPosition", "inNormal", "inTexCoord");

	printError("display");

	glutSwapBuffers();
}

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

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
