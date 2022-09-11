#include "Plane.h"
#include "../DebugMacros.h"

float plane_vertices[] = {
    // Location         // Texture coords
    -0.5f, 0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,// far left
     0.5f, 0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,// far rigt
     0.5f, 0.0f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,// near right
    -0.5f, 0.0f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f// near left
};

unsigned int plane_indices[] = {
    0, 1, 2,
    2, 3, 0
};

Plane::Plane(Shader* shader, bool instanced, bool isLight, bool useNormals)
    : Repeater(shader,
        instanced,
        plane_vertices,
        plane_indices,
        sizeof(plane_vertices) / sizeof(plane_vertices[0]),
        sizeof(plane_indices) / sizeof(plane_indices[0]),
        isLight,
        useNormals)
{
    std::cout << "Plane created!" << std::endl;
}

Plane::~Plane()
{
}
