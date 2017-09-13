#include <vector>

typedef struct {
  Model *m;
	vec3 pos, rot, sca;

  GLuint t[8], a[8]; //Texture color and alpha
  GLfloat tiles[8]; // Texture pattern repetitions

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
  for (int i=0; i<8; i++) {
    obj->t[i] = (i==0) ? white_texture : 0;
    obj->a[i] = (i==0) ? white_texture : 0;
    obj->tiles[i] = 1.0;
  }
  LoadTGATextureSimple("media/Tree/tree.tga", &(obj->t[0]));
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
  for (int i=0; i<8; i++) {
    obj->t[i] = (i==0) ? white_texture : 0;
    obj->a[i] = (i==0) ? white_texture : 0;
    obj->tiles[i] = 1.0;
  }
  LoadTGATextureSimple("media/Hawk/girl-body.tga", &(obj->t[0]));
  obj->pos = pos;
  obj->rot = rot;
  obj->sca = sca;
  obj->k_d = 1.0;
  obj->shininess = 1.0;
  obj->alpha = 1.0;
  objects.push_back(obj);

  obj = new StaticObject;
  obj->m = LoadModelPlus("media/Hawk/girl-head.obj");
  for (int i=0; i<8; i++) {
    obj->t[i] = (i==0) ? white_texture : 0;
    obj->a[i] = (i==0) ? white_texture : 0;
    obj->tiles[i] = 1.0;
  }
  LoadTGATextureSimple("media/Hawk/girl-head.tga", &(obj->t[0]));
  obj->pos = pos;
  obj->rot = rot;
  obj->sca = sca;
  obj->k_d = 1.0;
  obj->shininess = 1.0;
  obj->alpha = 1.0;
  objects.push_back(obj);

  obj = new StaticObject;
  obj->m = LoadModelPlus("media/Hawk/hawk-body.obj");
  for (int i=0; i<8; i++) {
    obj->t[i] = (i==0) ? white_texture : 0;
    obj->a[i] = (i==0) ? white_texture : 0;
    obj->tiles[i] = 1.0;
  }
  LoadTGATextureSimple("media/Hawk/hawk-body.tga", &(obj->t[0]));
  obj->pos = pos;
  obj->rot = rot;
  obj->sca = sca;
  obj->k_d = 1.0;
  obj->shininess = 1.0;
  obj->alpha = 1.0;
  objects.push_back(obj);

  obj = new StaticObject;
  obj->m = LoadModelPlus("media/Hawk/hawk-head.obj");
  for (int i=0; i<8; i++) {
    obj->t[i] = (i==0) ? white_texture : 0;
    obj->a[i] = (i==0) ? white_texture : 0;
    obj->tiles[i] = 1.0;
  }
  LoadTGATextureSimple("media/Hawk/hawk-head.tga", &(obj->t[0]));
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
  for (int i=0; i<8; i++) {
    obj->t[i] = (i==0) ? white_texture : 0;
    obj->a[i] = (i==0) ? white_texture : 0;
    obj->tiles[i] = 1.0;
  }
  //LoadTGATextureSimple("media/Teapot/rutor.tga", &(obj->t[0]));
  //LoadTGATextureSimple("media/Teapot/grass.tga", &(obj->t[1]));
  obj->a[1] = white_texture;
  obj->pos = SetVector(0,0,0);
  obj->pos.y = get_height(&terrain, obj->pos.x,obj->pos.z);
  obj->rot = SetVector(0,0,0);
  obj->sca = SetVector(1,1,1);
  obj->k_d = 1.0;
  obj->shininess = 20.0; //100.0;
  obj->alpha = 1.0;
  objects.push_back(obj);


  // Load a character

  obj = new StaticObject;
  obj->m = LoadModelPlus("media/Hestia/Hestia_forUpload.obj");
  for (int i=0; i<8; i++) {
    obj->t[i] = (i==0) ? white_texture : 0;
    obj->a[i] = (i==0) ? white_texture : 0;
    obj->tiles[i] = 1.0;
  }
  //LoadTGATextureSimple("media/Hestia/Hestia.tga", &(obj->t[0]));
  LoadTGATextureSimple("media/Hestia/[texture]Hestia_Face.tga", &(obj->t[0]));
  obj->a[1] = white_texture;
  obj->pos = SetVector(-100,0,0);
  obj->pos.y = get_height(&terrain, obj->pos.x,obj->pos.z);
  obj->rot = SetVector(0,-M_PI*0.5,0);
  obj->sca = SetVector(10,10,10);
  obj->k_d = 1.0;
  obj->shininess = 0.0; //100.0;
  obj->alpha = 1.0;
  objects.push_back(obj);
}


void draw_static_object(StaticObject *o) {
  mat4 transformationMatrix = transformation(o->pos, o->rot, o->sca);
  glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);

  GLint colorTexUnit[8], alphaTexUnit[8];
  GLfloat tilesTexUnit[8];
  for (int i=0; i<8; i++) {
    glActiveTexture(GL_TEXTURE0+i);
    glBindTexture(GL_TEXTURE_2D, o->t[i]);
    glActiveTexture(GL_TEXTURE0+i+8);
    glBindTexture(GL_TEXTURE_2D, o->a[i]);
    colorTexUnit[i] = i;
    alphaTexUnit[i] = 8+i;
    tilesTexUnit[i] = o->tiles[i];
  }
  glUniform1iv(glGetUniformLocation(program, "colorTexUnit"), 8, colorTexUnit);
  glUniform1iv(glGetUniformLocation(program, "alphaTexUnit"), 8, alphaTexUnit);
  glUniform1fv(glGetUniformLocation(program, "tilesTexUnit"), 8, tilesTexUnit);

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
  //o->pos = SetVector(x,y,z);
}
