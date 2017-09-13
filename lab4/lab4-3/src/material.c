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

void set_material(float shininess, float alpha, float k_d) {
  glUniform1f(glGetUniformLocation(program, "k_d"), k_d);
  glUniform1f(glGetUniformLocation(program, "specularExponent"), shininess);
  glUniform1f(glGetUniformLocation(program, "transparency"), alpha);
  if (alpha < 1.0) {
    glDisable(GL_CULL_FACE);
  } else {
    glEnable(GL_CULL_FACE);
  }
}
