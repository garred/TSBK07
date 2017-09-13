// Camera struct and functions

#define CAMERA_VELOCITY 1.0

typedef struct {
	vec3 p, l, v;
	vec3 mov, rot;
	mat4 lookAtMatrix;
} Camera;

Camera camera;


void init_camera() {
	Camera* c = &camera;

	c->p = SetVector(0,1,-3);
	c->l = SetVector(0,0,0);
	c->v = SetVector(0,1,0);
	c->mov = SetVector(0,0,0);
	c->rot = SetVector(0,0,0);
}


void update_camera(float t) {
	Camera *c = &camera;

	// User change camera position and rotation
	// Calculate the new rotation of the camera
	vec3 n = Normalize(VectorSub(c->l, c->p));
	vec3 x_axis = CrossProduct(n, SetVector(0,1,0));
	mat4 t_rot = Mult(ArbRotate(x_axis, -c->rot.y), Ry(-c->rot.x));
	n = MultVec3(t_rot, n);
	c->rot = SetVector(0,0,0);

	// Calculate the new position of the camera
	vec3 t_mov = VectorAdd(ScalarMult(x_axis, c->mov.x), VectorAdd(ScalarMult(n, c->mov.z), SetVector(0,c->mov.y,0)));
	c->p = VectorAdd(c->p, t_mov);
	c->mov = SetVector(0,0,0);

	// Update the look at position
	c->l = VectorAdd(c->p, n);

	// Calculate the `lookAtMatrix` and send to GPU device
	c->lookAtMatrix = lookAtv(c->p,c->l,c->v);
	glUniformMatrix4fv(glGetUniformLocation(program, "lookAtMatrix"), 1, GL_TRUE, c->lookAtMatrix.m);
}


// Input functions

void keyboard_func() {
	camera.mov.x = (glutKeyIsDown('d') - glutKeyIsDown('a')) * CAMERA_VELOCITY;
	camera.mov.z = (glutKeyIsDown('w') - glutKeyIsDown('s')) * CAMERA_VELOCITY;
	camera.mov.y = (glutKeyIsDown('r') - glutKeyIsDown('f')) * CAMERA_VELOCITY;

	// Resets the mouse position
	glutWarpPointer(200,200);
}

void mouse_func(int x, int y) {
	camera.rot.x = (x-200)*0.005;
	camera.rot.y = (y-200)*0.005;
}
