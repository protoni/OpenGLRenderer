#include "Cube.h"

#include "../DebugMacros.h"

float cube_vertices[] = {

    /* Bottom plane*/
    // Location         // Texture coords    // Normals
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        0.0f, -1.0f,  0.0f, // far left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        0.0f, -1.0f,  0.0f, // far rigt
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,        0.0f, -1.0f,  0.0f, // near right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,        0.0f, -1.0f,  0.0f, // near left

    /* Top plane*/
    // Location         // Texture coords
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,         0.0f,  1.0f,  0.0f,  // far left
     0.5f, 0.5f,  0.5f,  0.0f, 0.0f,         0.0f,  1.0f,  0.0f,  // far rigt
     0.5f, 0.5f, -0.5f,  0.0f, 0.0f,         0.0f,  1.0f,  0.0f,  // near right
    -0.5f, 0.5f, -0.5f,  0.0f, 0.0f,         0.0f,  1.0f,  0.0f,  // near left

    /* Left side plane */
   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,         -1.0f,  0.0f,  0.0f,  // bottom front
   -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,         -1.0f,  0.0f,  0.0f,  // bottom back
   -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,         -1.0f,  0.0f,  0.0f,  // top back
   -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,         -1.0f,  0.0f,  0.0f,  // top front

   /* Right side plane */
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f,          1.0f,  0.0f,  0.0f,  // bottom front
    0.5f, -0.5f,  0.5f, 0.0f, 0.0f,          1.0f,  0.0f,  0.0f,  // bottom back
    0.5f,  0.5f,  0.5f, 0.0f, 0.0f,          1.0f,  0.0f,  0.0f,  // top back
    0.5f,  0.5f, -0.5f, 0.0f, 0.0f,          1.0f,  0.0f,  0.0f,  // top front

    /* Front plane */
    0.5f,  0.5f, -0.5f,  0.0f, 0.0f,         0.0f,  0.0f, -1.0f,  // top right
    0.5f, -0.5f, -0.5f,  0.0f, 0.0f,         0.0f,  0.0f, -1.0f,  // bottom right
   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,         0.0f,  0.0f, -1.0f,  // bottom left
   -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,         0.0f,  0.0f, -1.0f,  // top left

   /* Back plane */
    0.5f,  0.5f,  0.5f,  0.0f, 0.0f,         0.0f,  0.0f,  1.0f,  // top right
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,         0.0f,  0.0f,  1.0f,  // bottom right
   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,         0.0f,  0.0f,  1.0f,  // bottom left
   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,         0.0f,  0.0f,  1.0f   // top left
};

unsigned int cube_indices[] = {

    /* Bottom plane */
    0, 1, 2,
    2, 3, 0,

    /* Top plane */
    4, 5, 6,
    6, 7, 4,

    /* Left side plane */
    8, 9, 10,
    10, 11, 8,

    /* Right side plane */
    12, 13, 14,
    14, 15, 12,

    /* Front plane */
    16, 17, 18,
    18, 19, 16,

    /* Back plane */
    20, 21, 22,
    22, 23, 20
};

Cube::Cube(Shader* shader, bool instanced, bool isLight, bool useNormals)
    : Repeater(shader,
        instanced,
        cube_vertices,
        cube_indices,
        sizeof(cube_vertices) / sizeof(cube_vertices[0]),
        sizeof(cube_indices) / sizeof(cube_indices[0]),
        isLight,
        useNormals)
{
    std::cout << "Cube created!" << std::endl;
}

Cube::~Cube()
{
}
