#include <vector>

typedef struct {
  Model *m;
	vec3 pos, rot, sca;
	GLuint t, t2;
  GLfloat k_d;
  GLfloat shininess;
  GLfloat alpha;
} StaticObject;

std::vector<StaticObject*> objects;

typedef struct {
  StaticObject static_object;
  int width, height;
} Terrain;

Terrain terrain;

float get_height(Terrain*, float,float);


// Load all the other models

void load_static_objects() {

  StaticObject* obj;


  // Load the tree

  obj = new StaticObject;
  obj->m = LoadModelPlus("media/Tree/tree.obj");
  LoadTGATextureSimple("media/Tree/tree.tga", &(obj->t));
  obj->t2 = 0;
  obj->pos = SetVector(0,0,0);
  obj->pos.y = get_height(&terrain, obj->pos.x,obj->pos.z);
  obj->rot = SetVector(0,0,M_PI*0.5);
  obj->sca = SetVector(1,2,2);
  obj->k_d = 1.0;
  obj->shininess = 1.0;
  obj->alpha = 1.0;
  objects.push_back(obj);


  // Load the models of the hawk and the girl

  vec3 pos = SetVector(50,30,-10);
  pos.y = get_height(&terrain, pos.x,pos.z) + 20;
  vec3 sca = SetVector(5,5,5);
  vec3 rot = SetVector(0,M_PI,0);

  obj = new StaticObject;
  obj->m = LoadModelPlus("media/Hawk/girl-body.obj");
  LoadTGATextureSimple("media/Hawk/girl-body.tga", &(obj->t));
  obj->t2 = 0;
  obj->pos = pos;
  obj->rot = rot;
  obj->sca = sca;
  obj->k_d = 1.0;
  obj->shininess = 1.0;
  obj->alpha = 1.0;
  objects.push_back(obj);

  obj = new StaticObject;
  obj->m = LoadModelPlus("media/Hawk/girl-head.obj");
  LoadTGATextureSimple("media/Hawk/girl-head.tga", &(obj->t));
  obj->t2 = 0;
  obj->pos = pos;
  obj->rot = rot;
  obj->sca = sca;
  obj->k_d = 1.0;
  obj->shininess = 1.0;
  obj->alpha = 1.0;
  objects.push_back(obj);

  obj = new StaticObject;
  obj->m = LoadModelPlus("media/Hawk/hawk-body.obj");
  LoadTGATextureSimple("media/Hawk/hawk-body.tga", &(obj->t));
  obj->t2 = 0;
  obj->pos = pos;
  obj->rot = rot;
  obj->sca = sca;
  obj->k_d = 1.0;
  obj->shininess = 1.0;
  obj->alpha = 1.0;
  objects.push_back(obj);

  obj = new StaticObject;
  obj->m = LoadModelPlus("media/Hawk/hawk-head.obj");
  LoadTGATextureSimple("media/Hawk/hawk-head.tga", &(obj->t));
  obj->t2 = 0;
  obj->pos = pos;
  obj->rot = rot;
  obj->sca = sca;
  obj->k_d = 1.0;
  obj->shininess = 1.0;
  obj->alpha = 1.0;
  objects.push_back(obj);


  // Load a teapot

  obj = new StaticObject;
  obj->m = LoadModelPlus("media/Teapot/teapot.obj");
  LoadTGATextureSimple("media/Teapot/rutor.tga", &(obj->t));
  LoadTGATextureSimple("media/Teapot/grass.tga", &(obj->t2));
  obj->pos = SetVector(0,0,0);
  obj->pos.y = get_height(&terrain, obj->pos.x,obj->pos.z);
  obj->rot = SetVector(0,0,0);
  obj->sca = SetVector(1,1,1);
  obj->k_d = 1.0;
  obj->shininess = 100.0;
  obj->alpha = 1;
  objects.push_back(obj);
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
  for (auto o : objects) {
    draw_static_object(o);
  }
}

void move_tree() {
  auto o = objects.back();
  float x,y,z;
  x = sin(t*0.1)*100.0;
  z = cos(t*0.1)*100.0;
  y = get_height(&terrain, x,z);
  o->pos = SetVector(x,y,z);
}
