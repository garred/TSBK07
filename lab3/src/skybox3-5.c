

typedef struct {
  Model *m;
  GLuint texture;
} Skybox;

Skybox skybox;


void load_skybox() {
  skybox.m = LoadModelPlus("skybox.obj");
  LoadTGATextureSimple("SkyBox512.tga", &(skybox.texture));
}


void draw_skybox() {
  mat4 transformationMatrix = transformation(camera.p, SetVector(0,0,0), SetVector(1,1,1));
  set_texture(skybox.texture);

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
  glUniform1f(glGetUniformLocation(program, "k_d"), 1.0);
  glUniform1f(glGetUniformLocation(program, "specularExponent"), 1.0);
  glUniform1f(glGetUniformLocation(program, "transparency"), 1.0);
  DrawModel(skybox.m, program, "inPosition", "inNormal", "inTexCoord");

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}
