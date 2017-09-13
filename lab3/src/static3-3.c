typedef struct {
  Model *m;
	vec3 pos, rot, sca;
	GLuint t;
} StaticObject;

#define OBJECTS 6
StaticObject obj[OBJECTS];


// Load all the other models

void load_static_objects() {

  // Load the ground
  obj[0].m = LoadModelPlus("lab2models/models/LittleNell/Tree/ground.obj");
  LoadTGATextureSimple("lab2models/grass.tga", &(obj[0].t));
  obj[0].pos = SetVector(0,0,0);
  obj[0].rot = SetVector(0,0,M_PI*0.5);
  obj[0].sca = SetVector(1,2,2);

  // Load the tree
  obj[1].m = LoadModelPlus("lab2models/models/LittleNell/Tree/tree.obj");
  LoadTGATextureSimple("lab2models/models/LittleNell/Tree/tree.tga", &(obj[1].t));
  obj[1].pos = SetVector(0,0,0);
  obj[1].rot = SetVector(0,0,M_PI*0.5);
  obj[1].sca = SetVector(1,2,2);


  // Load the models of the hawk and the girl
  vec3 pos = SetVector(50,30,-10);
  vec3 sca = SetVector(5,5,5);
  vec3 rot = SetVector(0,M_PI,0);

  obj[2].m = LoadModelPlus("lab2models/models/LittleNell/Hawk/girl-body.obj");
  LoadTGATextureSimple("lab2models/models/LittleNell/Hawk/girl-body.tga", &(obj[2].t));
  obj[2].pos = pos;
  obj[2].rot = rot;
  obj[2].sca = sca;

  obj[3].m = LoadModelPlus("lab2models/models/LittleNell/Hawk/girl-head.obj");
  LoadTGATextureSimple("lab2models/models/LittleNell/Hawk/girl-head.tga", &(obj[3].t));
  obj[3].pos = pos;
  obj[3].rot = rot;
  obj[3].sca = sca;

  obj[4].m = LoadModelPlus("lab2models/models/LittleNell/Hawk/hawk-body.obj");
  LoadTGATextureSimple("lab2models/models/LittleNell/Hawk/hawk-body.tga", &(obj[4].t));
  obj[4].pos = pos;
  obj[4].rot = rot;
  obj[4].sca = sca;

  obj[5].m = LoadModelPlus("lab2models/models/LittleNell/Hawk/hawk-head.obj");
  LoadTGATextureSimple("lab2models/models/LittleNell/Hawk/hawk-head.tga", &(obj[5].t));
  obj[5].pos = pos;
  obj[5].rot = rot;
  obj[5].sca = sca;
}


// Draw all the other models

void draw_static_objects() {
  mat4 transformationMatrix;

  // Draw the ground
  for (int i=0; i<OBJECTS; i++) {
    transformationMatrix = transformation(obj[i].pos, obj[i].rot, obj[i].sca);
    glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
    set_texture(obj[i].t);
    DrawModel(obj[i].m, program, "inPosition", "inNormal", "inTexCoord");
  }
}
