
typedef struct {
  GLint active;
  vec3 color;
  GLfloat range;
  GLfloat ambientFactor;
  GLint directional;
  vec3 pos_dir;
} Light;

#define N_LIGHTS 8
Light lights[N_LIGHTS];


void init_lights() {
  for (int i=0; i<N_LIGHTS; i++) {
    lights[i].active = 0;
    lights[i].color = SetVector(0,0,0);
    lights[i].range = 1.0;
    lights[i].ambientFactor = 0.0;
    lights[i].directional = 0;
    lights[i].pos_dir = SetVector(0,0,0);
  }

  lights[0].active = 1;
  lights[0].range = 200.0;
  lights[0].color = SetVector(1,0,0);
  lights[0].ambientFactor = 0.0;
  lights[0].directional = 0;
  lights[0].pos_dir = SetVector(0,1,0);

  lights[1].active = 1;
  lights[1].range = 200.0;
  lights[1].color = SetVector(0,1,0);
  lights[1].ambientFactor = 0.0;
  lights[1].directional = 0;
  lights[1].pos_dir = SetVector(0,1,0);

  lights[2].active = 1;
  lights[2].range = 200.0;
  lights[2].color = SetVector(0,0,1);
  lights[2].ambientFactor = 0.0;
  lights[2].directional = 0;
  lights[2].pos_dir = SetVector(0,0,0);


  lights[3].active = 1;
  lights[3].range = 1.0;
  lights[3].color = SetVector(1,0.5,0.0);
  lights[3].ambientFactor = 0.0;
  lights[3].directional = 1;
  lights[3].pos_dir = SetVector(1,0,0);

  lights[4].active = 1;
  lights[4].range = 1.0;
  lights[4].color = SetVector(0.0,0.0,0.1);
  lights[4].ambientFactor = 10;
  lights[4].directional = 1;
  lights[4].pos_dir = SetVector(0,1,0);

  lights[5].active = 1;
  lights[5].range = 1.0;
  lights[5].color = SetVector(0.75,0.75,0.75);
  lights[5].ambientFactor = 0.75;
  lights[5].directional = 1;
  lights[5].pos_dir = SetVector(1,-1,0);
}

void set_lights() {
  int i;

  for (i=0; i<3; i++) {
    float p = t+((M_PI*2)/3.0)*i;
    lights[i].pos_dir = SetVector(sin(p)*50.0, 50.0, cos(p)*50.0);
  }

  GLint isActive[N_LIGHTS];
  GLfloat lightSourcesColorsArr[N_LIGHTS*3];
  GLfloat lightRange[N_LIGHTS];
  GLfloat ambientFactor[N_LIGHTS];
  GLint isDirectional[N_LIGHTS];
  GLfloat lightSourcesDirectionsPositions[N_LIGHTS*3];

  for (i=0; i<N_LIGHTS; i++) {
    isActive[i] = lights[i].active;

    lightSourcesColorsArr[i*3+0] = lights[i].color.x;
    lightSourcesColorsArr[i*3+1] = lights[i].color.y;
    lightSourcesColorsArr[i*3+2] = lights[i].color.z;

    lightRange[i] = lights[i].range;

    ambientFactor[i] = lights[i].ambientFactor;

    isDirectional[i] = lights[i].directional;

    lightSourcesDirectionsPositions[i*3+0] = lights[i].pos_dir.x;
    lightSourcesDirectionsPositions[i*3+1] = lights[i].pos_dir.y;
    lightSourcesDirectionsPositions[i*3+2] = lights[i].pos_dir.z;
  }

  glUniform1iv(glGetUniformLocation(program, "isActive"), N_LIGHTS, isActive);
  glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), N_LIGHTS, lightSourcesDirectionsPositions);
  glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), N_LIGHTS, lightSourcesColorsArr);
  glUniform1fv(glGetUniformLocation(program, "lightRange"), N_LIGHTS, lightRange);
  glUniform1fv(glGetUniformLocation(program, "ambientFactor"), N_LIGHTS, ambientFactor);
  glUniform1iv(glGetUniformLocation(program, "isDirectional"), N_LIGHTS, isDirectional);
}
