// Windmill struct and functions

typedef struct {
	Model *blade, *walls, *roof, *balcony;
	float blade_angle;
	vec3 pos, rot, sca;
	mat4 blade_transform[4];
	GLuint texture[2];
} Windmill;

Windmill windmill;

void load_windmill() {
	Windmill* w = &windmill;

	w->blade = LoadModelPlus("media/windmill/blade.obj");
	w->walls = LoadModelPlus("media/windmill/windmill-walls.obj");
	w->roof = LoadModelPlus("media/windmill/windmill-roof.obj");
	w->balcony = LoadModelPlus("media/windmill/windmill-balcony.obj");
	w->blade_angle = 0;
	w->pos = SetVector(-10,0,90);
	w->pos.y = get_height(&terrain, w->pos.x, w->pos.z);
	w->rot = SetVector(0,0,0);
	w->sca = SetVector(2,2,2);
	// Pre-calculating the position and angle of each blade
	for (int i=0; i<4; i++) {
		w->blade_transform[i] = Mult(T(4.5,9.25,0), Rx(M_PI*i*0.5));
	}
	LoadTGATextureSimple("media/windmill/grass.tga", &(w->texture[0]));
	LoadTGATextureSimple("media/windmill/conc.tga", &(w->texture[1]));
}

void draw_windmill() {
	Windmill* w = &windmill;

	// Calculating global transformation of the windmill
	mat4 transformationMatrix = transformation(w->pos, w->rot, w->sca);

	glUniform1f(glGetUniformLocation(program, "k_d"), 1.0);
	glUniform1f(glGetUniformLocation(program, "specularExponent"), 1.0);
	glUniform1f(glGetUniformLocation(program, "transparency"), 1.0);

	// Drawing the blades
	set_texture(w->texture[0]);
	mat4 blade_rotation = Rx(w->blade_angle);
	for (int i=0; i<4; i++) {
		mat4 blade_transform = Mult(transformationMatrix, Mult(w->blade_transform[i], blade_rotation));
		glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, blade_transform.m);
		DrawModel(w->blade, program, "inPosition", "inNormal", "inTexCoord");
	}

	// Drawing the body
	set_texture(w->texture[1]);
	glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
	DrawModel(w->walls, program, "inPosition", "inNormal", "inTexCoord");
	DrawModel(w->roof, program, "inPosition", "inNormal", "inTexCoord");
	DrawModel(w->balcony, program, "inPosition", "inNormal", "inTexCoord");
}
