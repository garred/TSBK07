#ifndef _graphics_hpp_
#define _graphics_hpp_


#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"


#define NUMBER_LIGHTS 8


class Graphics {

public:
	static struct Frustum frustum_measures;
	static struct Light lights[NUMBER_LIGHTS];
	static GLuint shader_program;
	static mat4 projection_matrix;
	static GLuint white_texture;

	static void init_all();
	static void clear_all();
	static void draw_all();
	static void update_lights();

	static void set_texture(GLuint t);
	static void set_material(float shininess, float alpha, float k_d, bool useLights=true);

    static void load_media();
};




#endif
