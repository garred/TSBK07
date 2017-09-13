#ifndef _helpers_cpp_
#define _helpers_cpp_


#include <algorithm> //min, max
#include "VectorUtils3.h"


// Create a transformation matrix

mat4 transformation(vec3 pos, vec3 rot, vec3 sca) {
	mat4 t_pos, t_rot, t_sca;
	t_pos = T(pos.x, pos.y, pos.z);
	t_rot = Mult(Rx(rot.x), Mult(Ry(rot.y), Rz(rot.z)));
	t_sca = S(sca.x, sca.y, sca.z);
	return Mult(t_pos, Mult(t_rot, t_sca));
}


mat4 inverse_transformation(vec3 pos, vec3 rot, vec3 sca) {
	mat4 t_pos, t_rot, t_sca;
	t_pos = T(-pos.x, -pos.y, -pos.z);
	t_rot = Mult(Rx(-rot.x), Mult(Ry(-rot.y), Rz(-rot.z)));
	t_sca = S(1.0/sca.x, 1.0/sca.y, 1.0/sca.z);
	return Mult(t_pos, Mult(t_rot, t_sca));
}


mat4 set_mat_4(GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3,
			GLfloat p4, GLfloat p5, GLfloat p6, GLfloat p7,
			GLfloat p8, GLfloat p9, GLfloat p10, GLfloat p11,
			GLfloat p12, GLfloat p13, GLfloat p14, GLfloat p15
			)
{
	mat4 m;
	m.m[0] = p0;
	m.m[1] = p1;
	m.m[2] = p2;
	m.m[3] = p3;
	m.m[4] = p4;
	m.m[5] = p5;
	m.m[6] = p6;
	m.m[7] = p7;
	m.m[8] = p8;
	m.m[9] = p9;
	m.m[10] = p10;
	m.m[11] = p11;
	m.m[12] = p12;
	m.m[13] = p13;
	m.m[14] = p14;
	m.m[15] = p15;
	return m;
}


/*
mat4 look_at_transformation(vec3 position, vec3 look_at, vec3 v) {
	mat4 t_pos, t_rot;
	t_pos = T(position.x, position.y, position.z);

	vec3 n, u;
	n = Normalize(position - look_at);
	u = Normalize(CrossProduct(v, n));
	v = CrossProduct(n, u);
	t_rot = set_mat_4(u.x, u.y, u.z, 0,
              			v.x, v.y, v.z, 0,
                  	n.x, n.y, n.z, 0,
                  	0,   0,   0,   1);
	return Mult(t_pos, t_rot);
}
*/
mat4 look_at_transformation(vec3 O, vec3 P, vec3 U) {
	vec3 D = O - P;
	vec3 Right = Normalize(CrossProduct(U, D));
	vec3 Backwards = Normalize(CrossProduct(Right, U));
	vec3 Up = CrossProduct(Backwards, Right);
	return set_mat_4(Right.x, Right.y, Right.z, 0, Up.x, Up.y, Up.z, 0, Backwards.x, Backwards.y, Backwards.z, 0, 0, 0, 0, 1);

/*
	mat4 t_pos, t_rot;
	t_pos = T(position.x, position.y, position.z);

	vec3 n, u;
	n = Normalize(position - look_at);
	u = Normalize(CrossProduct(v, n));
	v = CrossProduct(n, u);
	t_rot = set_mat_4(u.x, u.y, u.z, 0,
              			v.x, v.y, v.z, 0,
                  	n.x, n.y, n.z, 0,
                  	0,   0,   0,   1);
	return Mult(t_pos, t_rot);
*/
}


#endif
