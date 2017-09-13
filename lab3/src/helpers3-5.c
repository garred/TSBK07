// Create a transformation matrix

mat4 transformation(vec3 pos, vec3 rot, vec3 sca) {
	mat4 t_pos, t_rot, t_sca;
	t_pos = T(pos.x, pos.y, pos.z);
	t_rot = Mult(Rx(rot.x), Mult(Ry(rot.y), Rz(rot.z)));
	t_sca = S(sca.x, sca.y, sca.z);
	return Mult(t_pos, Mult(t_rot, t_sca));
}


// Sets a texture active and send it to GPU

void set_texture(GLuint t) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, t);
  glUniform1i(glGetUniformLocation(program, "texUnit1"), 0); // Texture unit 0

	glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void set_multitexture(GLuint t1, GLuint t2) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, t1);
  glUniform1i(glGetUniformLocation(program, "texUnit1"), 0); // Texture unit 0

	glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, t2);
  glUniform1i(glGetUniformLocation(program, "texUnit2"), 1); // Texture unit 1
}
