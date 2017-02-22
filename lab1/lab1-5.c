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


// Globals
// Data would normally be read from files
GLfloat vertices[] =
{
	-0.5f, 0.5f,-0.5f,  -1.0f, 1.0f,-1.0f,1.0f,
	 0.5f, 0.5f,-0.5f,   1.0f, 1.0f,-1.0f,1.0f,
	-0.5f,-0.5f,-0.5f,  -1.0f,-1.0f,-1.0f,1.0f,
	 0.5f,-0.5f,-0.5f,   1.0f,-1.0f,-1.0f,1.0f,
	-0.5f, 0.5f, 0.5f,  -1.0f, 1.0f, 1.0f,1.0f,
	 0.5f, 0.5f, 0.5f,   1.0f, 1.0f, 1.0f,1.0f,
	-0.5f,-0.5f, 0.5f,  -1.0f,-1.0f, 1.0f,1.0f,
	 0.5f,-0.5f, 0.5f,	 1.0f,-1.0f, 1.0f,1.0f
 };

GLuint triangles[] =
{
	0,4,2,
	4,6,2,
	0,2,3,
	0,3,1,
	2,6,3,
	3,6,7,
	1,3,7,
	1,7,5,
	4,7,6,
	4,5,7,
	0,1,4,
	4,1,5
};

Matrix4D myMatrix;

// Reference to shader program
GLuint program;


// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
	printError("init shader");

	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);


	// Allocate Vertex Buffer Objects

	// VBO for cube vertices and colors
	// VBO for vertex data
	unsigned int vertexBufferObjID;
	glGenBuffers(1, &vertexBufferObjID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 7*8*sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	GLint posAttrib = glGetAttribLocation(program, "in_Position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), 0);

	GLint colAttrib = glGetAttribLocation(program, "in_Color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(3*sizeof(float)));

	unsigned int elementBufferObjID;
	glGenBuffers(1, &elementBufferObjID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

	// End of upload of geometry

	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Sending transformation matrix to the vertex shader
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	myMatrix = Mult(Ry(t*0.002), Rx(t*0.001));
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_FALSE, myMatrix.m);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawElements(GL_TRIANGLES, 3*12, GL_UNSIGNED_INT, 0);

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
