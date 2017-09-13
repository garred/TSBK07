#ifndef _entities_cpp_
#define _entities_cpp_

#include "entities.hpp"
#include "graphics.hpp"
#include "game.hpp"
#include "random.hpp"
#include "helpers.hpp"

#include <iostream>
//#include <gl_mangle.h>

using namespace std;


/*******************************************************************************
********************************************************************************
*******************************************************************************/

Camera *Camera::current;

void Camera::update() {
    // User change camera position and rotation
    // Calculate the new rotation of the camera
    vec3 n = Normalize(l - position);
    vec3 x_axis = CrossProduct(n, SetVector(0, 1, 0));
    mat4 t_rot = ArbRotate(x_axis, -rotation.y) * Ry(-rotation.x);
    n = MultVec3(t_rot, n);
    rotation = SetVector(0, 0, 0);

    // Calculate the new position of the camera
    vec3 t_mov = x_axis * movement.x + n * movement.z + SetVector(0, movement.y, 0);
    position = position + t_mov;
    movement = SetVector(0, 0, 0);

    // Update the look at position
    l = position + n;

    // Calculate the `lookAtMatrix` and send to GPU device
    lookAtMatrix = lookAtv(position, l, v);
    glUniformMatrix4fv(
            glGetUniformLocation(Graphics::shader_program, "lookAtMatrix"),
            1, GL_TRUE, lookAtMatrix.m);

    // We also send the camera position in world coordinates
    GLfloat p[3];
    p[0] = position.x;
    p[1] = position.y;
    p[2] = position.z;
    glUniform3fv(glGetUniformLocation(Graphics::shader_program, "cameraWorldPos"), 1, p);

}


/*******************************************************************************
********************************************************************************
*******************************************************************************/

Terrain *Terrain::current = NULL;


void generate_terrain(Terrain *terrain, TextureData *tex) {
    int vertexCount = tex->width * tex->height;
    int triangleCount = (tex->width - 1) * (tex->height - 1) * 2;
    int x, z;

    GLfloat *vertexArray = (GLfloat *) malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *n = (GLfloat *) malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *normalArray = (GLfloat *) malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *texCoordArray = (GLfloat *) malloc(sizeof(GLfloat) * 2 * vertexCount);
    GLuint *indexArray = (GLuint *) malloc(sizeof(GLuint) * triangleCount * 3);

    printf("bpp %d\n", tex->bpp);
    for (x = 0; x < (int) tex->width; x++)
        for (z = 0; z < (int) tex->height; z++) {
// Vertex array. You need to scale this properly
            vertexArray[(x + z * tex->width) * 3 + 0] = x / 1.0;
            vertexArray[(x + z * tex->width) * 3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp / 8)] * 0.1;
            vertexArray[(x + z * tex->width) * 3 + 2] = z / 1.0;
// Texture coordinates. You may want to scale them.
            texCoordArray[(x + z * tex->width) * 2 + 0] = (float) x / tex->width;
            texCoordArray[(x + z * tex->width) * 2 + 1] = (float) z / tex->height;
            //texCoordArray[(x + z * tex->width)*2 + 0] = (float)x;
            //texCoordArray[(x + z * tex->width)*2 + 1] = (float)z;
        }

    // To calculate the normals, first we calculate for each vertex the normal of
    // the two triangles that are placed at down-right in the grid.
    for (x = 0; x < (int) tex->width - 1; x++)
        for (z = 0; z < (int) tex->height - 1; z++) {
            // Hard-coded normals calculation. You are not supposed to understand this
            // code.
            float a[3], b[3], c[3], d[3];    //Position of the vertices of interest (d is also reused to calculate vector lengths)
            float ab[3], ac[3], ad[3];        //Normalized directions between vertices of interest

            // Getting vertex info
            a[0] = vertexArray[(x + z * tex->width) * 3 + 0];
            a[1] = vertexArray[(x + z * tex->width) * 3 + 1];
            a[2] = vertexArray[(x + z * tex->width) * 3 + 2];
            b[0] = vertexArray[((x + 1) + z * tex->width) * 3 + 0];
            b[1] = vertexArray[((x + 1) + z * tex->width) * 3 + 1];
            b[2] = vertexArray[((x + 1) + z * tex->width) * 3 + 2];
            c[0] = vertexArray[((x + 1) + (z + 1) * tex->width) * 3 + 0];
            c[1] = vertexArray[((x + 1) + (z + 1) * tex->width) * 3 + 1];
            c[2] = vertexArray[((x + 1) + (z + 1) * tex->width) * 3 + 2];
            d[0] = vertexArray[(x + (z + 1) * tex->width) * 3 + 0];
            d[1] = vertexArray[(x + (z + 1) * tex->width) * 3 + 1];
            d[2] = vertexArray[(x + (z + 1) * tex->width) * 3 + 2];

            // Calculating segments connecting vertices
            ab[0] = b[0] - a[0];
            ab[1] = b[1] - a[1];
            ab[2] = b[2] - a[2];
            ac[0] = c[0] - a[0];
            ac[1] = c[1] - a[1];
            ac[2] = c[2] - a[2];
            ad[0] = d[0] - a[0];
            ad[1] = d[1] - a[1];
            ad[2] = d[2] - a[2];

            // Normalizing segments to obtain directions
            d[0] = 1.0 / sqrt(ab[0] * ab[0] + ab[1] * ab[1] + ab[2] * ab[2]);
            d[1] = 1.0 / sqrt(ac[0] * ac[0] + ac[1] * ac[1] + ac[2] * ac[2]);
            d[2] = 1.0 / sqrt(ad[0] * ad[0] + ad[1] * ad[1] + ad[2] * ad[2]);
            ab[0] *= d[0];
            ab[1] *= d[0];
            ab[2] *= d[0];
            ac[0] *= d[1];
            ac[1] *= d[1];
            ac[2] *= d[1];
            ad[0] *= d[2];
            ad[1] *= d[2];
            ad[2] *= d[2];

            // Performing crossproducts. Here I reuse variables a and b as the vectors
            // which I want to crossproduct, just for "clarity"

            // Crossproduct 1
            a[0] = ab[0];
            a[1] = ab[1];
            a[2] = ab[2];
            b[0] = ac[0];
            b[1] = ac[1];
            b[2] = ac[2];
            n[(x + z * tex->width) * 3 + 0] = a[1] * b[2] - a[2] * b[1];
            n[(x + z * tex->width) * 3 + 1] = a[2] * b[0] - a[0] * b[2];
            n[(x + z * tex->width) * 3 + 2] = a[0] * b[1] - a[1] * b[0];

            // Crossproduct 2
            a[0] = ac[0];
            a[1] = ac[1];
            a[2] = ac[2];
            b[0] = ad[0];
            b[1] = ad[1];
            b[2] = ad[2];
            n[(x + z * tex->width) * 3 + 0] += a[1] * b[2] - a[2] * b[1];
            n[(x + z * tex->width) * 3 + 1] += a[2] * b[0] - a[0] * b[2];
            n[(x + z * tex->width) * 3 + 2] += a[0] * b[1] - a[1] * b[0];

            // Mean normal
            n[(x + z * tex->width) * 3 + 0] *= 0.5;
            n[(x + z * tex->width) * 3 + 1] *= 0.5;
            n[(x + z * tex->width) * 3 + 2] *= 0.5;
        }

    // Now we calculate the real normal of each vertex by adding the precalculated
    // pairs of normals.
    for (x = 0; x < (int) tex->width; x++)
        for (z = 0; z < (int) tex->height; z++) {
            float vertex_count = 0.0;
            normalArray[(x + z * tex->width) * 3 + 0] = 0.0;
            normalArray[(x + z * tex->width) * 3 + 1] = 0.0;
            normalArray[(x + z * tex->width) * 3 + 2] = 0.0;

            for (int i = std::max(0, x - 1); i <= x; i++) {
                for (int j = std::max(0, z - 1); j <= z; j++) {
                    vertex_count += 1.0;
                    normalArray[(x + z * tex->width) * 3 + 0] += n[(i + j * tex->width) * 3 + 0];
                    normalArray[(x + z * tex->width) * 3 + 1] += n[(i + j * tex->width) * 3 + 1];
                    normalArray[(x + z * tex->width) * 3 + 2] += n[(i + j * tex->width) * 3 + 2];
                }
            }
            vertex_count = 1.0 / vertex_count;
            normalArray[(x + z * tex->width) * 3 + 0] *= -vertex_count;
            normalArray[(x + z * tex->width) * 3 + 1] *= -vertex_count;
            normalArray[(x + z * tex->width) * 3 + 2] *= -vertex_count;
        }


    for (x = 0; x < (int) tex->width - 1; x++)
        for (z = 0; z < (int) tex->height - 1; z++) {
            // Triangle 1
            indexArray[(x + z * (tex->width - 1)) * 6 + 0] = x + z * tex->width;
            indexArray[(x + z * (tex->width - 1)) * 6 + 1] = x + (z + 1) * tex->width;
            indexArray[(x + z * (tex->width - 1)) * 6 + 2] = x + 1 + z * tex->width;
            // Triangle 2
            indexArray[(x + z * (tex->width - 1)) * 6 + 3] = x + 1 + z * tex->width;
            indexArray[(x + z * (tex->width - 1)) * 6 + 4] = x + (z + 1) * tex->width;
            indexArray[(x + z * (tex->width - 1)) * 6 + 5] = x + 1 + (z + 1) * tex->width;
        }

    // End of terrain generation

    // Create Model and upload to GPU:

    Model *model = LoadDataToModel(
            vertexArray,
            normalArray,
            texCoordArray,
            NULL,
            indexArray,
            vertexCount,
            triangleCount * 3);

    terrain->model = model;
    terrain->width = tex->width;
    terrain->height = tex->height;
}


void Terrain::load_terrain() {
    TextureData heightmap_texture;
    LoadTGATextureData("media/terrain/heightmap.tga", &heightmap_texture);
    generate_terrain(this, &heightmap_texture);

    for (int i = 0; i < 8; i++) {
        this->texture_color[i] = (i == 0) ? Graphics::white_texture : 0;
        this->texture_alpha[i] = (i == 0) ? Graphics::white_texture : 0;
        this->texture_tile[i] = 30.0;
    }
    LoadTGATextureSimple("media/terrain/water.tga", &(this->texture_color[0]));
    LoadTGATextureSimple("media/terrain/level_0.tga", &(this->texture_alpha[0]));
    LoadTGATextureSimple("media/terrain/sand.tga", &(this->texture_color[1]));
    LoadTGATextureSimple("media/terrain/level_1.tga", &(this->texture_alpha[1]));
    LoadTGATextureSimple("media/terrain/grass.tga", &(this->texture_color[2]));
    LoadTGATextureSimple("media/terrain/level_2.tga", &(this->texture_alpha[2]));
    LoadTGATextureSimple("media/terrain/rock-grass.tga", &(this->texture_color[3]));
    LoadTGATextureSimple("media/terrain/level_3.tga", &(this->texture_alpha[3]));
    LoadTGATextureSimple("media/terrain/rock.tga", &(this->texture_color[4]));
    LoadTGATextureSimple("media/terrain/level_4.tga", &(this->texture_alpha[4]));

    this->position = SetVector(-2560, 0, -2560);
    this->scale = SetVector(20, 20, 20);
    this->rotation = SetVector(0, 0, 0);
    this->reflectivity = 1.0;
    this->shininess = 0.0;
    this->alpha = 1.0;
}


// I'm assumming that terrains are never rotated (but they could be scaled or translated).
float Terrain::get_height(vec3 object_position) {
    float x = object_position.x;
    float z = object_position.z;

    x = (x - position.x) / scale.x;
    z = (z - position.z) / scale.z;

    // Calculate the cell
    int ix = floor(x);
    int iz = floor(z);

    // If the point is not in the terrain, we have finished.
    if (ix < 0 || ix >= width || iz < 0 || iz >= height)
        return 0.0;

    // Calculate in what triangle does the point fall
    float fx, fz;
    fx = x - ix;
    fz = z - iz;
    int t[3];
    // Triangle 1
    if (fx + fz <= 1) {
        t[0] = ix + iz * width;
        t[1] = ix + (iz + 1) * width;
        t[2] = ix + 1 + iz * width;
        // Triangle 2
    } else {
        t[0] = ix + 1 + iz * width;
        t[1] = ix + (iz + 1) * width;
        t[2] = ix + 1 + (iz + 1) * width;
    }

    // Calculate the point in the triangle
    GLfloat *vertexArray = model->vertexArray;

    vec3 t1, t2, t3, n;
    t1 = SetVector(vertexArray[t[0] * 3 + 0], vertexArray[t[0] * 3 + 1], vertexArray[t[0] * 3 + 2]);
    t2 = SetVector(vertexArray[t[1] * 3 + 0], vertexArray[t[1] * 3 + 1], vertexArray[t[1] * 3 + 2]);
    t3 = SetVector(vertexArray[t[2] * 3 + 0], vertexArray[t[2] * 3 + 1], vertexArray[t[2] * 3 + 2]);
    n = CrossProduct(t1 - t2, t1 - t3);
    float D = -DotProduct(n, t1);
    float mu = (-D - DotProduct(n, SetVector(x, 0, z))) / n.y;
    return mu * scale.y;
}


// I'm assumming that terrains are never rotated (but they could be scaled or translated).
vec3 Terrain::get_normal(vec3 object_position) {
    float x = object_position.x;
    float z = object_position.z;

    x = (x - position.x) / scale.x;
    z = (z - position.z) / scale.z;

    // Calculate the cell
    int ix = floor(x);
    int iz = floor(z);

    // If the point is not in the terrain, we have finished.
    if (ix < 0 || ix >= width || iz < 0 || iz >= height)
        return SetVector(0, 1, 0);

    // Calculate in what triangle does the point fall
    float fx, fz;
    fx = x - ix;
    fz = z - iz;
    int t[3];
    // Triangle 1
    if (fx + fz <= 1) {
        t[0] = ix + iz * width;
        t[1] = ix + (iz + 1) * width;
        t[2] = ix + 1 + iz * width;
        // Triangle 2
    } else {
        t[0] = ix + 1 + iz * width;
        t[1] = ix + (iz + 1) * width;
        t[2] = ix + 1 + (iz + 1) * width;
    }

    // Calculate the point in the triangle
    GLfloat *vertexArray = model->vertexArray;

    vec3 t1, t2, t3, n;
    t1 = SetVector(vertexArray[t[0] * 3 + 0], vertexArray[t[0] * 3 + 1], vertexArray[t[0] * 3 + 2]);
    t2 = SetVector(vertexArray[t[1] * 3 + 0], vertexArray[t[1] * 3 + 1], vertexArray[t[1] * 3 + 2]);
    t3 = SetVector(vertexArray[t[2] * 3 + 0], vertexArray[t[2] * 3 + 1], vertexArray[t[2] * 3 + 2]);
    n = CrossProduct(t1 - t2, t1 - t3);
    return n;
}


/*******************************************************************************
********************************************************************************
*******************************************************************************/

void Node::update() {
    velocity = velocity + SetVector(0, -9.8, 0) * Game::delta;
    position = position + velocity;

    float terrain_height = Terrain::current->get_height(position);
    if ((position.y - size) < terrain_height) {
        vec3 terrain_normal = Terrain::current->get_normal(position);

        position.y = terrain_height + size;
        vec3 bounce_velocity, slidding_velocity;
        SplitVector(velocity, terrain_normal, &bounce_velocity, &slidding_velocity);
        velocity = slidding_velocity * slideness - bounce_velocity * bounceness;
    }
}


void Node::draw()
{ //TODO
    /*
    vec3 n = Normalize(l - position);
    vec3 x_axis = CrossProduct(n, SetVector(0,1,0));
    mat4 t_rot = ArbRotate(x_axis, -rotation.y) * Ry(-rotation.x);
    n = MultVec3(t_rot, n);
    rotation = SetVector(0,0,0);
    */

    // Calculating 3 contact points to the ground to calculate the vertical vector
    vec3 fl,fr,bl,br; //front-left, front-right, back-left, back-right
    vec3 dir = Normalize(SetVector(direction.x, 0, direction.z));
    vec3 side = SetVector(-dir.z, 0, dir.x);
    fl = position + dir - side;
    fr = position + dir + side;
    bl = position - dir - side;
    br = position - dir + side;

    // Calculating the position
    fl.y = Terrain::current->get_height(fl);
    fr.y = Terrain::current->get_height(fr);
    bl.y = Terrain::current->get_height(bl);
    br.y = Terrain::current->get_height(br);
    position = (fl + fr + bl + br) * 0.25;

    // Calculating the normal
    vec3 up = (Terrain::current->get_normal(fl) +
            Terrain::current->get_normal(fr) +
            Terrain::current->get_normal(bl) +
            Terrain::current->get_normal(br)) * 0.25;

    vec3 right = CrossProduct(up, dir);
    dir = CrossProduct(up, right);

    up = Normalize(up);
    right = Normalize(right);
    dir = Normalize(dir);

    mat4 lt = IdentityMatrix();
    lt.m[0]=dir.x;  lt.m[1]=dir.y;  lt.m[2]=dir.z;
    lt.m[4]=up.x;   lt.m[5]=up.y;   lt.m[6]=up.z;
    lt.m[8]=right.x;lt.m[9]=right.y;lt.m[10]=right.z;
    lt = Transpose(lt);

    transformation_matrix = transformation(position, rotation, scale) * lt;

    glUniformMatrix4fv(glGetUniformLocation(Graphics::shader_program, "transformationMatrix"), 1, GL_TRUE,
                       transformation_matrix.m);

    GLint colorTexUnit[8], alphaTexUnit[8];
    GLfloat tilesTexUnit[8];
    for (int i = 0; i < 8; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture_color[i]);
        glActiveTexture(GL_TEXTURE0 + i + 8);
        glBindTexture(GL_TEXTURE_2D, texture_alpha[i]);
        colorTexUnit[i] = i;
        alphaTexUnit[i] = 8 + i;
        tilesTexUnit[i] = texture_tile[i];
    }
    glUniform1iv(glGetUniformLocation(Graphics::shader_program, "colorTexUnit"), 8, colorTexUnit);
    glUniform1iv(glGetUniformLocation(Graphics::shader_program, "alphaTexUnit"), 8, alphaTexUnit);
    glUniform1fv(glGetUniformLocation(Graphics::shader_program, "tilesTexUnit"), 8, tilesTexUnit);

    Graphics::set_material(shininess, alpha, reflectivity);
    DrawModel(model, Graphics::shader_program, "inPosition", "inNormal", "inTexCoord");
}


#endif
