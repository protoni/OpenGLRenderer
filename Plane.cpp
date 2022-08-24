#include "Plane.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

float plane_vertices[] = {
    // Location         // Texture coords
    -0.5f, 0.0f,  0.5f,  0.0f, 0.0f, // far left
     0.5f, 0.0f,  0.5f,  0.0f, 0.0f, // far rigt
     0.5f, 0.0f, -0.5f,  0.0f, 0.0f, // near right
    -0.5f, 0.0f, -0.5f,  0.0f, 0.0f, // near left
};

unsigned int plane_indices[] = {
    0, 1, 2,
    2, 3, 0
};

Plane::Plane(Shader* shader)
: Mesh(shader,
    plane_vertices,
    plane_indices,
    sizeof(plane_vertices) / sizeof(plane_vertices[0]),
    sizeof(plane_indices) / sizeof(plane_indices[0]),
    6
  ), m_rows(1), m_columns(1), m_scale(1.0)
{
}

Plane::~Plane()
{
}

void Plane::update(int rows, int columns, float scale)
{
    m_rows = rows;
    m_columns = columns;
    m_scale = scale;
}

void Plane::draw()
{
    activate();

    for (int j = 0; j < m_rows; j++) { // rows
        for (int i = 0; i < m_columns; i++) { // columns
            render(i, j, m_scale);
        }
    }
}
