// Create a transformation matrix

mat4 transformation(vec3 pos, vec3 rot, vec3 sca) {
	mat4 t_pos, t_rot, t_sca;
	t_pos = T(pos.x, pos.y, pos.z);
	t_rot = Mult(Rx(rot.x), Mult(Ry(rot.y), Rz(rot.z)));
	t_sca = S(sca.x, sca.y, sca.z);
	return Mult(t_pos, Mult(t_rot, t_sca));
}
