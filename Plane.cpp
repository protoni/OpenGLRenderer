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

Plane::Plane(Shader* shader, bool instanced)
: Mesh(shader,
    plane_vertices,
    plane_indices,
    sizeof(plane_vertices) / sizeof(plane_vertices[0]),
    sizeof(plane_indices) / sizeof(plane_indices[0])
  ), m_rows(1), m_columns(1), m_scale(1.0), m_buffer(0), m_matrices(NULL), m_shader(shader), m_instanced(instanced),
    m_padding(0.0f)
{

    // Init instance buffer
    if(m_instanced)
        createBuffer();
}

Plane::Plane(Shader* shader, bool instanced, float* vertices, unsigned int* indices, unsigned int verticeCount, unsigned int indiceCount)
    : Mesh(shader,
        vertices,
        indices,
        verticeCount,
        indiceCount
    ), m_rows(1), m_columns(1), m_scale(1.0), m_buffer(0), m_matrices(NULL), m_shader(shader), m_instanced(instanced)
{
    std::cout << "vertex count: " << verticeCount << ", index count: " << indiceCount << std::endl;

    // Init instance buffer
    if (m_instanced)
        createBuffer();
}

Plane::~Plane()
{
    if(m_matrices)
        delete[] m_matrices;
}

// Create instanced buffer on the GPU
void Plane::createBuffer()
{   
    int ptr = 0;

    delete[] m_matrices;

    m_matrices = new glm::mat4[m_rows * m_columns];
    for (int j = 0; j < m_rows; j++) { // rows
        for (int i = 0; i < m_columns; i++) { // columns
            glm::mat4* model = getMesh(i, j, m_scale, m_padding);
            if (model)
                m_matrices[ptr++] = *model;
        }
    }

    std::cout << "planes: " << m_rows * m_columns << std::endl;

    glGenBuffers(1, &m_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_rows * m_columns * sizeof(glm::mat4), &m_matrices[0], GL_STATIC_DRAW);

    activate();
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    deactivate();
}

void Plane::update(int rows, int columns, float scale, float padding)
{
    m_rows = rows;
    m_columns = columns;
    m_scale = scale;
    m_padding = padding;

    if (m_instanced)
        createBuffer();
}

void Plane::drawNonInstanced()
{
    activate();

    for (int j = 0; j < m_rows; j++) { // rows
        for (int i = 0; i < m_columns; i++) { // columns
            render(i, j, m_scale, m_padding);
        }
    }

    deactivate();
}

void Plane::drawInstanced(int faceCount)
{
    activate();
    glDrawElementsInstanced(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0, (m_rows * m_columns));
    deactivate();
}

