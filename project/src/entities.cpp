#ifndef _entities_cpp_
#define _entities_cpp_

#include "entities.hpp"
#include "graphics.hpp"
#include "game.hpp"
#include "random.hpp"
#include "helpers.hpp"

#include <iostream>
#include <tuple>
#include <functional>

using namespace std;


/*******************************************************************************
********************************************************************************
*******************************************************************************/

Camera *Camera::current;

void Camera::update() {
    // If the camera is following an object, we update its coordinates
    if (following) {
        vec3 destination = following->get_global_matrix() * relative_position;
        position = (1-following_velocity)*position + following_velocity*destination;
        l = following->get_global_matrix() * (following->position + relative_look_at);
    }

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
    LoadTGATextureData("media/random_terrain/heightmap.tga", &heightmap_texture);
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


// Membership functions to generate the alpha levels of terrain types from the height info.
double membership(double value, double bottom_left, double top_left, double top_right, double bottom_right) {
    if (value < bottom_left)
        return 0.0;
    else if (value < top_left)
        return (value-bottom_left) / (top_left-bottom_left);
    else if (value < top_right)
        return 1.0;
    else if (value < bottom_right)
        return 1.0 - ((value-top_right) / (bottom_right-top_right));
    else
        return 0.0;
}
using namespace std::placeholders;
auto water_func     = std::bind(membership, _1, -1.0, 0.0, 0.0, 0.005);
auto sand_func      = std::bind(membership, _1,  0.0, 0.005, 0.1, 0.4);
auto grass_func     = std::bind(membership, _1,  0.1, 0.4, 0.5, 0.6);
auto rockgrass_func = std::bind(membership, _1,  0.5, 0.6, 0.7, 0.8);
auto rock_func      = std::bind(membership, _1,  0.7, 0.8, 1.0, 2.0);

void create_texture(TextureData& texture) {
    texture.bpp = 24;
    texture.width = 256;
    texture.height = 256;
    texture.w = 256;
    texture.h = 256;
    texture.texID = 0;
    texture.texWidth = 1;
    texture.texHeight = 1;

    GLuint image_size = (texture.bpp/8) * texture.w * texture.h;
    texture.imageData = new GLubyte[image_size];
}

void generate_height_map(double (&m)[256][256]) {

    // Initial random values
    for (unsigned int x = 0; x < 256; x++) {
        for (unsigned int y = 0; y < 256; y++) {
            m[x][y] = Random::real(0, 1);
        }
    }

    // Smoothing 100 times
    for (int i = 0; i < 100; i++) {
        double u[256][256];
        double max = -10000, min = 10000;
        for (int x = 0; x < 256; x++) {
            for (int y = 0; y < 256; y++) {
                double n = 0.0;
                u[x][y] = 0.0;
                for (int j = -1; j <= 1; j++) {
                    for (int k = -1; k <= 1; k++) {
                        if ((x + j) >= 0 and (x + j) < 256 and
                            (y + k) >= 0 and (y + k) < 256) {
                            n += 1.0;
                            u[x][y] += m[x + j][y + k];
                        }
                    }
                }
                u[x][y] /= n;
                max = (u[x][y] > max) ? u[x][y] : max;
                min = (u[x][y] < min) ? u[x][y] : min;
            }
        }
        for (unsigned int x = 0; x < 256; x++)
            for (unsigned int y = 0; y < 256; y++)
                m[x][y] = (u[x][y] - min) / (max - min);
    }

    // Creating some water areas
    double max = -1000;
    for (int x = 0; x < 256; x++)
        for (int y = 0; y < 256; y++) {
            m[x][y] = m[x][y] - 0.3;
            m[x][y] = (m[x][y] < 0.0) ? 0.0 : m[x][y];
            max = (m[x][y] > max) ? m[x][y] : max;
        }
    max = 1.0 / max;
    for (int x = 0; x < 256; x++)
        for (int y = 0; y < 256; y++)
            m[x][y] *= max;
}



void process_height_data(double (&height_data)[256][256], double (&texture_data)[256][256], decltype(water_func) f) {
    for (int x=0; x<256; x++)
        for (int y=0; y<256; y++)
            texture_data[x][y] = f(height_data[x][y]);
}


void data_to_texture(double (&data)[256][256], TextureData& texture) {
    for (unsigned int x=0; x<256; x++) {
        for (unsigned int y=0; y<256; y++) {
            texture.imageData[x*3 + y*256*3    ] = GLubyte(data[x][y]*255);
            texture.imageData[x*3 + y*256*3 + 1] = texture.imageData[x*3 + y*256*3];
            texture.imageData[x*3 + y*256*3 + 2] = texture.imageData[x*3 + y*256*3];
        }
    }
}

void Terrain::randomize() {

    // Create the data structure of each texture

    TextureData height_texture;
    TextureData water_texture;
    TextureData sand_texture;
    TextureData grass_texture;
    TextureData rockgrass_texture;
    TextureData rock_texture;
    create_texture(height_texture);
    create_texture(water_texture);
    create_texture(sand_texture);
    create_texture(grass_texture);
    create_texture(rockgrass_texture);
    create_texture(rock_texture);

    // Generate a smoothed random terrain

    double height_data[256][256];
    generate_height_map(height_data);

    // Generate the alpha data for each texture

    double water_data[256][256];
    double sand_data[256][256];
    double grass_data[256][256];
    double rockgrass_data[256][256];
    double rock_data[256][256];
    process_height_data(height_data, water_data,     water_func);
    process_height_data(height_data, sand_data,      sand_func);
    process_height_data(height_data, grass_data,     grass_func);
    process_height_data(height_data, rockgrass_data, rockgrass_func);
    process_height_data(height_data, rock_data,      rock_func);

    // Put data inside textures

    data_to_texture(height_data,    height_texture);//, true);
    data_to_texture(water_data,     water_texture);
    data_to_texture(sand_data,      sand_texture);
    data_to_texture(grass_data,     grass_texture);
    data_to_texture(rockgrass_data, rockgrass_texture);
    data_to_texture(rock_data,      rock_texture);

    // Save the textures

    SaveTGA(&height_texture,    "media/random_terrain/heightmap.tga");
    SaveTGA(&water_texture,     "media/random_terrain/level_0.tga");
    SaveTGA(&sand_texture,      "media/random_terrain/level_1.tga");
    SaveTGA(&grass_texture,     "media/random_terrain/level_2.tga");
    SaveTGA(&rockgrass_texture, "media/random_terrain/level_3.tga");
    SaveTGA(&rock_texture,      "media/random_terrain/level_4.tga");

    // Generate the polygons

    LoadTGATextureData("media/random_terrain/heightmap.tga", &height_texture);
    generate_terrain(this, &height_texture);

    // Add textures

    for (int i = 0; i < 8; i++) {
        this->texture_color[i] = (i == 0) ? Graphics::white_texture : 0;
        this->texture_alpha[i] = (i == 0) ? Graphics::white_texture : 0;
        this->texture_tile[i] = 30.0;
    }
    LoadTGATextureSimple("media/random_terrain/water.tga", &(this->texture_color[0]));
    LoadTGATextureSimple("media/random_terrain/level_0.tga", &(this->texture_alpha[0]));
    LoadTGATextureSimple("media/random_terrain/sand.tga", &(this->texture_color[1]));
    LoadTGATextureSimple("media/random_terrain/level_1.tga", &(this->texture_alpha[1]));
    LoadTGATextureSimple("media/random_terrain/grass.tga", &(this->texture_color[2]));
    LoadTGATextureSimple("media/random_terrain/level_2.tga", &(this->texture_alpha[2]));
    LoadTGATextureSimple("media/random_terrain/rock-grass.tga", &(this->texture_color[3]));
    LoadTGATextureSimple("media/random_terrain/level_3.tga", &(this->texture_alpha[3]));
    LoadTGATextureSimple("media/random_terrain/rock.tga", &(this->texture_color[4]));
    LoadTGATextureSimple("media/random_terrain/level_4.tga", &(this->texture_alpha[4]));

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


Model *Vehicle::body_model, *Vehicle::turret_model, *Vehicle::cannon_model;
GLuint Vehicle::model_texture;


tuple<vec3,vec3,vec3,vec3> Vehicle::_update_ground() {
    // Calculating contact points to the ground to calculate the vertical vector
    vec3 fl,fr,bl,br; //front-left, front-right, back-left, back-right
    vec3 dir = Normalize(SetVector(direction.x, 0, direction.z));
    vec3 side = SetVector(-dir.z, 0, dir.x);
    fl = position + dir*6 - side*3;
    fr = position + dir*6 + side*3;
    bl = position - dir*6 - side*3;
    br = position - dir*6 + side*3;

    // Calculating the position
    fl.y = Terrain::current->get_height(fl);
    fr.y = Terrain::current->get_height(fr);
    bl.y = Terrain::current->get_height(bl);
    br.y = Terrain::current->get_height(br);

    ground = (fl + fr + bl + br) * 0.25;

    return make_tuple(fl, fr, bl, br);
}

mat4 Vehicle::_get_terrain_orientation() {
    vec3 fl,fr,bl,br; //front-left, front-right, back-left, back-right
    tie(fl, fr, bl, br) = _update_ground();

    // Calculating the normal
    vec3 up = (Terrain::current->get_normal(fl) +
               Terrain::current->get_normal(fr) +
               Terrain::current->get_normal(bl) +
               Terrain::current->get_normal(br)) * 0.25;
    vec3 pre_normal = normal;
    double v = 0.01;
    normal = Normalize(up*v + normal*(1-v) + normal_vel*v);
    normal_vel = normal_vel*(1-v) + normal - pre_normal;
    up = normal;

    vec3 dir = direction;

    // Calculating the right vector
    vec3 right = CrossProduct(up, dir);


    // Updating the dir vector and normalizing
    dir = Normalize(CrossProduct(up, right));
    up = Normalize(up);
    right = Normalize(right);

    // Calculating the transformation matrix
    mat4 lt = IdentityMatrix();
    lt.m[0]=dir.x;  lt.m[1]=up.x;   lt.m[2] =right.x;
    lt.m[4]=dir.y;  lt.m[5]=up.y;   lt.m[6] =right.y;
    lt.m[8]=dir.z;  lt.m[9]=up.z;   lt.m[10]=right.z;

    return lt;
}


void Vehicle::update() {

    _update_ground();

    // Friction
    vec3 terrain_normal = Terrain::current->get_normal(position);

    position.y = ground.y;

    velocity += movement;
    vec3 bounce_velocity, slidding_velocity;
    SplitVector(velocity, terrain_normal, &bounce_velocity, &slidding_velocity);
    velocity = slidding_velocity * slideness - bounce_velocity * bounceness;

    position += velocity;

}


mat4 Vehicle::get_transformation_matrix() {
    return transformation(position, rotation, scale) * _get_terrain_orientation();
}


/*******************************************************************************
********************************************************************************
*******************************************************************************/

Model* Bullet::bullet_model;

void Bullet::update() {
    // Move the bullet
    position += velocity * Game::delta;
    velocity += SetVector(0.0,-9.8,0.0) * Game::delta * 10.0;

    // Check if it hits the terrain
    if (position.y < Terrain::current->get_height(position)) {
        Explosion::create(position);
        alive = false;
    }
}

/*******************************************************************************
********************************************************************************
*******************************************************************************/

Model* Explosion::explosion_model;
GLuint Explosion::fire_texture;

void Explosion::update() {
    alpha -= 0.025;
    life -= 5;
    scale *= 1.25;

    // Check if the explosion died
    if (life <= 0.0) {
        alive = false;
    }
}


#endif
