#ifndef _graphics_cpp_
#define _graphics_cpp_


#include "graphics.hpp"


struct Light {
  GLint active;
  vec3 color;
  GLfloat range;
  GLfloat ambientFactor;
  GLint directional;
  vec3 pos_dir;
} Graphics::lights[NUMBER_LIGHTS];


struct Frustum {
  float near = 0.1;
  float far = 10000.0;
  float right = 0.05;
  float left = -0.05;
  float top = 0.05;
  float bottom = -0.05;
} Graphics::frustum_measures;

GLuint Graphics::shader_program;
mat4 Graphics::projection_matrix;
GLuint Graphics::white_texture;


void create_window() {
  glutInit(NULL, NULL);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitContextVersion(3, 2);
  glutInitWindowSize(800,800);
  glutCreateWindow ("Tank Battle!");
}


void init_opengl() {
  glClearColor(0.2,0.2,0.5,1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glFrontFace(GL_CCW);
  printError("GL inits");
}

void reset_textures() {
  for (int i=0; i<16; i++) {
    glActiveTexture(GL_TEXTURE0+i);
    glBindTexture(GL_TEXTURE_2D, Graphics::white_texture);
  }
}


void init_lights() {
  for (int i=0; i<NUMBER_LIGHTS; i++) {
    Graphics::lights[i].active = 0;
    Graphics::lights[i].color = SetVector(0,0,0);
    Graphics::lights[i].range = 1.0;
    Graphics::lights[i].ambientFactor = 0.0;
    Graphics::lights[i].directional = 0;
    Graphics::lights[i].pos_dir = SetVector(0,0,0);
  }

  Graphics::lights[0].active = 1;
  Graphics::lights[0].range = 200.0;
  Graphics::lights[0].color = SetVector(1,1,1);
  Graphics::lights[0].ambientFactor = 0.0;
  Graphics::lights[0].directional = 1;
  Graphics::lights[0].pos_dir = SetVector(0,-1,0);
}


// Create a window, set initial configuration
void Graphics::init_all() {
  create_window();
  dumpInfo();
  init_opengl();

	// Load and compile shader
	Graphics::shader_program = loadShaders("src/lab.vert", "src/lab.frag");
	printError("init shader");

	// Set and send projection matrix to the vertex shader
	Graphics::projection_matrix = frustum(
    Graphics::frustum_measures.left,    Graphics::frustum_measures.right,
    Graphics::frustum_measures.bottom,  Graphics::frustum_measures.top,
    Graphics::frustum_measures.near,    Graphics::frustum_measures.far);
	glUniformMatrix4fv(
    glGetUniformLocation(Graphics::shader_program, "projectionMatrix"),
    1, GL_TRUE, Graphics::projection_matrix.m);

  LoadTGATextureSimple("media/white.tga", &Graphics::white_texture);
  reset_textures();

  init_lights();
}


void Graphics::clear_all() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


// Send the light info to the GPU
void Graphics::update_lights() {
  int i;

  GLint isActive[NUMBER_LIGHTS];
  GLfloat lightSourcesColorsArr[NUMBER_LIGHTS*3];
  GLfloat lightRange[NUMBER_LIGHTS];
  GLfloat ambientFactor[NUMBER_LIGHTS];
  GLint isDirectional[NUMBER_LIGHTS];
  GLfloat lightSourcesDirectionsPositions[NUMBER_LIGHTS*3];

  for (i=0; i<NUMBER_LIGHTS; i++) {
    isActive[i] = Graphics::lights[i].active;

    lightSourcesColorsArr[i*3+0] = Graphics::lights[i].color.x;
    lightSourcesColorsArr[i*3+1] = Graphics::lights[i].color.y;
    lightSourcesColorsArr[i*3+2] = Graphics::lights[i].color.z;

    lightRange[i] = Graphics::lights[i].range;

    ambientFactor[i] = Graphics::lights[i].ambientFactor;

    isDirectional[i] = Graphics::lights[i].directional;

    lightSourcesDirectionsPositions[i*3+0] = Graphics::lights[i].pos_dir.x;
    lightSourcesDirectionsPositions[i*3+1] = Graphics::lights[i].pos_dir.y;
    lightSourcesDirectionsPositions[i*3+2] = Graphics::lights[i].pos_dir.z;
  }

  glUniform1iv(glGetUniformLocation(Graphics::shader_program, "isActive"), NUMBER_LIGHTS, isActive);
  glUniform3fv(glGetUniformLocation(Graphics::shader_program, "lightSourcesDirPosArr"), NUMBER_LIGHTS, lightSourcesDirectionsPositions);
  glUniform3fv(glGetUniformLocation(Graphics::shader_program, "lightSourcesColorArr"), NUMBER_LIGHTS, lightSourcesColorsArr);
  glUniform1fv(glGetUniformLocation(Graphics::shader_program, "lightRange"), NUMBER_LIGHTS, lightRange);
  glUniform1fv(glGetUniformLocation(Graphics::shader_program, "ambientFactor"), NUMBER_LIGHTS, ambientFactor);
  glUniform1iv(glGetUniformLocation(Graphics::shader_program, "isDirectional"), NUMBER_LIGHTS, isDirectional);
}


void Graphics::draw_all() {
  glutSwapBuffers();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}




void Graphics::set_texture(GLuint t) {
  GLint colorTexUnit[8], alphaTexUnit[8];
  for (int i=0; i<8; i++) {
    glActiveTexture(GL_TEXTURE0+i);
    glBindTexture(GL_TEXTURE_2D, (i==0) ? t : 0);
    glActiveTexture(GL_TEXTURE8+i);
    glBindTexture(GL_TEXTURE_2D, (i==0) ? Graphics::white_texture : 0);
    colorTexUnit[i] = i;
    alphaTexUnit[i] = i+8;
  }
  glUniform1iv(glGetUniformLocation(Graphics::shader_program, "colorTexUnit"), 8, colorTexUnit);
  glUniform1iv(glGetUniformLocation(Graphics::shader_program, "alphaTexUnit"), 8, alphaTexUnit);
}


void Graphics::set_material(float shininess, float alpha, float k_d) {
  glUniform1f(glGetUniformLocation(Graphics::shader_program, "k_d"), k_d);
  glUniform1f(glGetUniformLocation(Graphics::shader_program, "specularExponent"), shininess);
  glUniform1f(glGetUniformLocation(Graphics::shader_program, "transparency"), alpha);
  if (alpha < 1.0) {
    glDisable(GL_CULL_FACE);
  } else {
    glEnable(GL_CULL_FACE);
  }
}



#endif
