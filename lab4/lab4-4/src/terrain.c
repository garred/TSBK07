
void GenerateTerrain(Terrain *terrain, TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;

	GLfloat *vertexArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *n = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = (GLfloat*)malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = (GLuint*)malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < (int)tex->width; x++)
		for (z = 0; z < (int)tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] * 0.1;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = (float)z / tex->height;
      //texCoordArray[(x + z * tex->width)*2 + 0] = (float)x;
			//texCoordArray[(x + z * tex->width)*2 + 1] = (float)z;
		}

	// To calculate the normals, first we calculate for each vertex the normal of
	// the two triangles that are placed at down-right in the grid.
	for (x = 0; x < (int)tex->width-1; x++)
		for (z = 0; z < (int)tex->height-1; z++)
		{
			// Hard-coded normals calculation. You are not supposed to understand this
			// code.
			float a[3], b[3], c[3], d[3];	//Position of the vertices of interest (d is also reused to calculate vector lengths)
			float ab[3], ac[3], ad[3];		//Normalized directions between vertices of interest

			// Getting vertex info
			a[0] = vertexArray[(x + z * tex->width)*3 + 0];
			a[1] = vertexArray[(x + z * tex->width)*3 + 1];
			a[2] = vertexArray[(x + z * tex->width)*3 + 2];
			b[0] = vertexArray[((x+1) + z * tex->width)*3 + 0];
			b[1] = vertexArray[((x+1) + z * tex->width)*3 + 1];
			b[2] = vertexArray[((x+1) + z * tex->width)*3 + 2];
			c[0] = vertexArray[((x+1) + (z+1) * tex->width)*3 + 0];
			c[1] = vertexArray[((x+1) + (z+1) * tex->width)*3 + 1];
			c[2] = vertexArray[((x+1) + (z+1) * tex->width)*3 + 2];
			d[0] = vertexArray[(x + (z+1) * tex->width)*3 + 0];
			d[1] = vertexArray[(x + (z+1) * tex->width)*3 + 1];
			d[2] = vertexArray[(x + (z+1) * tex->width)*3 + 2];

			// Calculating segments connecting vertices
			ab[0] = b[0]-a[0];
			ab[1] = b[1]-a[1];
			ab[2] = b[2]-a[2];
			ac[0] = c[0]-a[0];
			ac[1] = c[1]-a[1];
			ac[2] = c[2]-a[2];
			ad[0] = d[0]-a[0];
			ad[1] = d[1]-a[1];
			ad[2] = d[2]-a[2];

			// Normalizing segments to obtain directions
			d[0] = 1.0 / sqrt(ab[0]*ab[0] + ab[1]*ab[1] + ab[2]*ab[2]);
			d[1] = 1.0 / sqrt(ac[0]*ac[0] + ac[1]*ac[1] + ac[2]*ac[2]);
			d[2] = 1.0 / sqrt(ad[0]*ad[0] + ad[1]*ad[1] + ad[2]*ad[2]);
			ab[0]*=d[0]; ab[1]*=d[0]; ab[2]*=d[0];
			ac[0]*=d[1]; ac[1]*=d[1]; ac[2]*=d[1];
			ad[0]*=d[2]; ad[1]*=d[2]; ad[2]*=d[2];

			// Performing crossproducts. Here I reuse variables a and b as the vectors
			// which I want to crossproduct, just for "clarity"

			// Crossproduct 1
			a[0]=ab[0]; a[1]=ab[1]; a[2]=ab[2]; b[0]=ac[0]; b[1]=ac[1]; b[2]=ac[2];
			n[(x + z * tex->width)*3 + 0] = a[1]*b[2] - a[2]*b[1];
			n[(x + z * tex->width)*3 + 1] = a[2]*b[0] - a[0]*b[2];
			n[(x + z * tex->width)*3 + 2] = a[0]*b[1] - a[1]*b[0];

			// Crossproduct 2
			a[0]=ac[0]; a[1]=ac[1]; a[2]=ac[2]; b[0]=ad[0]; b[1]=ad[1]; b[2]=ad[2];
			n[(x + z * tex->width)*3 + 0] += a[1]*b[2] - a[2]*b[1];
			n[(x + z * tex->width)*3 + 1] += a[2]*b[0] - a[0]*b[2];
			n[(x + z * tex->width)*3 + 2] += a[0]*b[1] - a[1]*b[0];

			// Mean normal
			n[(x + z * tex->width)*3 + 0] *= 0.5;
			n[(x + z * tex->width)*3 + 1] *= 0.5;
			n[(x + z * tex->width)*3 + 2] *= 0.5;
		}

	// Now we calculate the real normal of each vertex by adding the precalculated
	// pairs of normals.
	for (x = 0; x < (int)tex->width; x++)
		for (z = 0; z < (int)tex->height; z++)
		{
			float vertex_count = 0.0;
			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 0.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;

			for (int i=std::max(0,x-1); i<=x; i++) {
				for (int j=std::max(0,z-1); j<=z; j++) {
					vertex_count += 1.0;
					normalArray[(x + z * tex->width)*3 + 0] += n[(i + j * tex->width)*3 + 0];
					normalArray[(x + z * tex->width)*3 + 1] += n[(i + j * tex->width)*3 + 1];
					normalArray[(x + z * tex->width)*3 + 2] += n[(i + j * tex->width)*3 + 2];
				}
			}
			vertex_count = 1.0 / vertex_count;
			normalArray[(x + z * tex->width)*3 + 0] *= -vertex_count;
			normalArray[(x + z * tex->width)*3 + 1] *= -vertex_count;
			normalArray[(x + z * tex->width)*3 + 2] *= -vertex_count;
		}


	for (x = 0; x < (int)tex->width-1; x++)
		for (z = 0; z < (int)tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	terrain->static_object.m = model;
  terrain->width = tex->width;
  terrain->height = tex->height;
}


void load_terrain() {
  TextureData heightmap_texture;
  //LoadTGATextureData("media/Terrain/heightmap.tga", &heightmap_texture);
  LoadTGATextureData("media/Terrain/fft-terrain.tga", &heightmap_texture);
  GenerateTerrain(&terrain, &heightmap_texture);
  LoadTGATextureSimple("media/Terrain/maskros512.tga", &(terrain.static_object.t));
  terrain.static_object.t2 = 0;
  //terrain.static_object.pos = SetVector(0,0,0);
  terrain.static_object.pos = SetVector(-1280,0,-1280);
  terrain.static_object.sca = SetVector(10,10,10);
  terrain.static_object.rot = SetVector(0,0,0);
  terrain.static_object.k_d = 1.0;
  terrain.static_object.shininess = 0.0;
  terrain.static_object.alpha = 1.0;
}


void draw_terrain() {
  draw_static_object(&(terrain.static_object));
}


// I'm assumming that terrains are never rotated (but they could be scaled or translated).
float get_height(Terrain* terrain, float x, float z) {
  x = (x - terrain->static_object.pos.x) / terrain->static_object.sca.x;
  z = (z - terrain->static_object.pos.z) / terrain->static_object.sca.z;

  // Calculate the cell
  int ix = floor(x);
  int iz = floor(z);

  // If the point is not in the terrain, we have finished.
  if (ix<0 || ix>=terrain->width || iz<0 || iz>=terrain->height)
    return 0.0;

  // Calculate in what triangle does the point fall
  float fx, fz;
  fx = x - ix;
  fz = z - iz;
  int t[3];
  // Triangle 1
  if (fx+fz <= 1) {
			t[0] = ix + iz * terrain->width;
			t[1] = ix + (iz+1) * terrain->width;
			t[2] = ix+1 + iz * terrain->width;
  // Triangle 2
  } else {
			t[0] = ix+1 + iz * terrain->width;
			t[1] = ix + (iz+1) * terrain->width;
			t[2] = ix+1 + (iz+1) * terrain->width;
  }

  // Calculate the point in the triangle
  GLfloat* vertexArray = terrain->static_object.m->vertexArray;

  vec3 t1,t2,t3, n;
  t1 = SetVector(vertexArray[t[0]*3 + 0], vertexArray[t[0]*3 + 1], vertexArray[t[0]*3 + 2]);
  t2 = SetVector(vertexArray[t[1]*3 + 0], vertexArray[t[1]*3 + 1], vertexArray[t[1]*3 + 2]);
  t3 = SetVector(vertexArray[t[2]*3 + 0], vertexArray[t[2]*3 + 1], vertexArray[t[2]*3 + 2]);
  n = CrossProduct(VectorSub(t1,t2), VectorSub(t1,t3));
  float D = -DotProduct(n,t1);
  float mu = (-D - DotProduct(n,SetVector(x,0,z))) / n.y;
  return mu*terrain->static_object.sca.y;
}
