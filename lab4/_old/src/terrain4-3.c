#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *n = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] * 0.1;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = (float)z / tex->height;
		}

	// To calculate the normals, first we calculate for each vertex the normal of
	// the two triangles that are placed at down-right in the grid.
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
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
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			float vertex_count = 0.0;
			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 0.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;

			for (int i=max(0,x-1); i<=x; i++) {
				for (int j=max(0,z-1); j<=z; j++) {
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


	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
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

	return model;
}


void draw_terrain() {
  set_material(1,1,1);
  set_texture(tex1);
  //glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
  DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
}
