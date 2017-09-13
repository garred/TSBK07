GLuint white_texture;

void reset_textures() {
  for (int i=0; i<16; i++) {
    glActiveTexture(GL_TEXTURE0+i);
    glBindTexture(GL_TEXTURE_2D, white_texture);
  }
}

void init_textures() {
  LoadTGATextureSimple("media/white.tga", &white_texture);
  reset_textures();
}

void set_texture(GLuint t) {
  GLint colorTexUnit[8], alphaTexUnit[8];
  for (int i=0; i<8; i++) {
    glActiveTexture(GL_TEXTURE0+i);
    glBindTexture(GL_TEXTURE_2D, (i==0) ? t : 0);
    glActiveTexture(GL_TEXTURE8+i);
    glBindTexture(GL_TEXTURE_2D, (i==0) ? white_texture : 0);
    colorTexUnit[i] = i;
    alphaTexUnit[i] = i+8;
  }
  glUniform1iv(glGetUniformLocation(program, "colorTexUnit"), 8, colorTexUnit);
  glUniform1iv(glGetUniformLocation(program, "alphaTexUnit"), 8, alphaTexUnit);
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
