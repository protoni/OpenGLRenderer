#include "Repeater.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

Repeater::Repeater(Shader* shader, bool instanced, float* vertices, unsigned int* indices, unsigned int verticeCount, unsigned int indiceCount)
    : Mesh(shader,
        vertices,
        indices,
        verticeCount,
        indiceCount
    ), m_buffer(0), m_matrices(NULL), m_shader(shader), m_state()
{
    std::cout << "vertex count: " << verticeCount << ", index count: " << indiceCount << std::endl;

    // Init state
    m_state = new RepeaterState();

    // Set initial instancing state
    m_state->instanced = instanced;

    // Init instance buffer
    if (m_state->instanced)
        createBuffer();
}

Repeater::~Repeater()
{
    if(m_matrices)
        delete[] m_matrices;

    if (m_state)
        delete m_state;
}

// Create instanced buffer on the GPU
void Repeater::createBuffer()
{   
    int ptr = 0;

    delete[] m_matrices;

    m_matrices = new glm::mat4[getObjCount()];
    for (int y = 0; y < m_state->stackCount; y++) {          // stacks  ( y-axis )
        for (int z = 0; z < m_state->rowCount; z++) {        // rows    ( z axis )
            for (int x = 0; x < m_state->columnCount; x++) { // columns ( x axis )
                m_matrices[ptr++] = *getMesh(x, y, z, m_state->scale, m_state->padding);
            }
        }
    }

    std::cout << "planes: " << (getObjCount()) << std::endl;

    glGenBuffers(1, &m_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, (getObjCount()) * sizeof(glm::mat4), &m_matrices[0], GL_STATIC_DRAW);

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

int Repeater::getObjCount()
{
    return m_state->rowCount * m_state->columnCount * m_state->stackCount;
}

RepeaterState* Repeater::getState()
{
    return m_state;
}

void Repeater::update()
{
    if (m_state->instanced)
        createBuffer();
}

void Repeater::drawNonInstanced()
{
    activate();

    for (int y = 0; y < m_state->stackCount; y++) {          // stacks  ( y-axis )
        for (int z = 0; z < m_state->rowCount; z++) {        // rows    ( z axis )
            for (int x = 0; x < m_state->columnCount; x++) { // columns ( x axis )
                render(x, y, z, m_state->scale, m_state->padding);
            }
        }
    }

    deactivate();
}

void Repeater::drawInstanced(int faceCount)
{
    activate();
    glDrawElementsInstanced(GL_TRIANGLES, (6 * faceCount), GL_UNSIGNED_INT, 0, getObjCount());
    deactivate();
}

