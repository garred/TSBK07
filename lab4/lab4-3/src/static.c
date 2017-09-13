typedef struct {
  Model *m;
	vec3 pos, rot, sca;
	GLuint t, t2;
  GLfloat k_d;
  GLfloat shininess;
  GLfloat alpha;
} StaticObject;

#define OBJECTS 7
StaticObject obj[OBJECTS];


// Load all the other models

void load_static_objects() {

  // Load the ground

  obj[0].m = LoadModelPlus("media/Tree/ground.obj");
  LoadTGATextureSimple("media/Tree/grass.tga", &(obj[0].t));
  obj[0].t2 = 0;
  obj[0].pos = SetVector(0,0,0);
  obj[0].rot = SetVector(0,0,M_PI*0.5);
  obj[0].sca = SetVector(1,2,2);
  obj[0].k_d = 1.0;
  obj[0].shininess = 1.0;
  obj[0].alpha = 1.0;


  // Load the tree

  obj[1].m = LoadModelPlus("media/Tree/tree.obj");
  LoadTGATextureSimple("media/Tree/tree.tga", &(obj[1].t));
  obj[1].t2 = 0;
  obj[1].pos = SetVector(0,0,0);
  obj[1].rot = SetVector(0,0,M_PI*0.5);
  obj[1].sca = SetVector(1,2,2);
  obj[1].k_d = 1.0;
  obj[1].shininess = 1.0;
  obj[1].alpha = 1.0;


  // Load the models of the hawk and the girl

  vec3 pos = SetVector(50,30,-10);
  vec3 sca = SetVector(5,5,5);
  vec3 rot = SetVector(0,M_PI,0);

  obj[2].m = LoadModelPlus("media/Hawk/girl-body.obj");
  LoadTGATextureSimple("media/Hawk/girl-body.tga", &(obj[2].t));
  obj[2].t2 = 0;
  obj[2].pos = pos;
  obj[2].rot = rot;
  obj[2].sca = sca;
  obj[2].k_d = 1.0;
  obj[2].shininess = 1.0;
  obj[2].alpha = 1.0;

  obj[3].m = LoadModelPlus("media/Hawk/girl-head.obj");
  LoadTGATextureSimple("media/Hawk/girl-head.tga", &(obj[3].t));
  obj[3].t2 = 0;
  obj[3].pos = pos;
  obj[3].rot = rot;
  obj[3].sca = sca;
  obj[3].k_d = 1.0;
  obj[3].shininess = 1.0;
  obj[3].alpha = 1.0;

  obj[4].m = LoadModelPlus("media/Hawk/hawk-body.obj");
  LoadTGATextureSimple("media/Hawk/hawk-body.tga", &(obj[4].t));
  obj[4].t2 = 0;
  obj[4].pos = pos;
  obj[4].rot = rot;
  obj[4].sca = sca;
  obj[4].k_d = 1.0;
  obj[4].shininess = 1.0;
  obj[4].alpha = 1.0;

  obj[5].m = LoadModelPlus("media/Hawk/hawk-head.obj");
  LoadTGATextureSimple("media/Hawk/hawk-head.tga", &(obj[5].t));
  obj[5].t2 = 0;
  obj[5].pos = pos;
  obj[5].rot = rot;
  obj[5].sca = sca;
  obj[5].k_d = 1.0;
  obj[5].shininess = 1.0;
  obj[5].alpha = 1.0;


  // Load a teapot

  obj[6].m = LoadModelPlus("media/Teapot/teapot.obj");
  LoadTGATextureSimple("media/Teapot/rutor.tga", &(obj[6].t));
  LoadTGATextureSimple("media/Teapot/grass.tga", &(obj[6].t2));
  obj[6].pos = SetVector(0,0,0);
  obj[6].rot = SetVector(0,0,0);
  obj[6].sca = SetVector(1,1,1);
  obj[6].k_d = 1.0;
  obj[6].shininess = 100.0;
  obj[6].alpha = 1;
}


void draw_static_object(StaticObject *o) {
  mat4 transformationMatrix = transformation(o->pos, o->rot, o->sca);
  glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
  if (o->t2==0) {
    set_texture(o->t);
  } else {
    set_multitexture(o->t, o->t2);
  }
  set_material(o->shininess, o->alpha, o->k_d);
  DrawModel(o->m, program, "inPosition", "inNormal", "inTexCoord");
}

// Draw all the static models

void draw_static_objects() {
  for (int i=0; i<OBJECTS; i++) {
    draw_static_object(&(obj[i]));
  }
}
