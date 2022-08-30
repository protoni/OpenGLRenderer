#include "Triangle.h"
#include "../DebugMacros.h"

float triangle_vertices[] = {
    // Location         // Texture coords
    -0.5f, 0.0f,  0.5f,  0.0f, 0.0f, // far left
     0.5f, 0.0f,  0.5f,  0.0f, 0.0f, // far rigt
     0.5f, 0.0f, -0.5f,  0.0f, 0.0f  // near right

};

unsigned int triangle_indices[] = {
    0, 1, 2
};

Triangle::Triangle(Shader* shader, bool instanced)
    : Repeater(shader,
        instanced,
        triangle_vertices,
        triangle_indices,
        sizeof(triangle_vertices) / sizeof(triangle_vertices[0]),
        sizeof(triangle_indices) / sizeof(triangle_indices[0]))
{
    std::cout << "Triangle created!" << std::endl;
}

Triangle::~Triangle()
{
}
