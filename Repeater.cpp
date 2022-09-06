#include "Repeater.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "DebugMacros.h"

Repeater::Repeater(Shader* shader, bool instanced, float* vertices, unsigned int* indices, unsigned int verticeCount, unsigned int indiceCount)
    : Mesh(shader,
        vertices,
        indices,
        verticeCount,
        indiceCount
    ), m_buffer(0), m_matrices(NULL), m_shader(shader), m_state(), m_indiceCount(indiceCount), m_deleteRemoved(0)
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

Repeater::Repeater(Shader* shader, bool instanced)
    : Mesh(shader), m_buffer(0), m_matrices(NULL), m_shader(shader), m_state(), m_indiceCount(0)
{
    // Init state
    m_state = new RepeaterState();

    // Set initial instancing state
    m_state->instanced = instanced;

    // Init instance buffer
    if (m_state->instanced)
        createBuffer();

    cleanup();
}

Repeater::~Repeater()
{
    std::cout << "Repeater destructor called!" << std::endl;
    if(m_matrices)
        delete[] m_matrices;

    if (m_state) {
        delete m_state;
        m_state = NULL;
    }
}

void Repeater::setIndiceCount(unsigned int count)
{
    m_indiceCount = count;
}

bool Repeater::meshDeleted(int meshPointer)
{
    bool ret = false;
    if (m_state->deleted) {
        std::vector<int> deletedMeshes = *m_state->deleted;
        if (std::find(deletedMeshes.begin(), deletedMeshes.end(), meshPointer) != deletedMeshes.end())
            ret = true;

        for (int i = 0; i < deletedMeshes.size(); i++) {
        }
    }

    return ret;
}

// Create instanced buffer on the GPU
void Repeater::createBuffer()
{   
    int ptr = 0;

    delete[] m_matrices;

    // Get deleted mesh count
    int deletedCount = 0;
    if (m_state->deleted)
        deletedCount = m_state->deleted->size();

    //int meshPointer = 0;
    bool removedLastTime = false;
    m_matrices = new glm::mat4[getObjCount()];
    for (int y = 0; y < m_state->stackCount; y++) {          // stacks  ( y-axis )
        for (int z = 0; z < m_state->rowCount; z++) {        // rows    ( z axis )
            for (int x = 0; x < m_state->columnCount; x++) {  // columns ( x axis )
                if (!meshDeleted(ptr)) {
                    m_matrices[ptr++] = *getMesh(x, y, z, m_state, false, 0);
                }
                else {
                    m_matrices[ptr++] = *getMesh(x, y, z, m_state, true, m_deleteRemoved);
                }
                //if (!meshDeleted(ptr)) {
                //    if (removedLastTime) {
                //        removedLastTime = false;
                //        m_matrices[ptr++] = *getMesh(x+1, y, z, m_state, true, m_deleteRemoved);
                //    }
                //    else
                //        m_matrices[ptr++] = *getMesh(x, y, z, m_state, false, 0);
                //}
                //else {
                //    removedLastTime = true;
                //}
                //else if(meshDeleted(ptr))
                    //    m_matrices[ptr++] = *getMesh(x, y, z, m_state, false, 0);
                //if (meshDeleted(ptr)) {
                //    m_matrices[ptr++] = *getMesh(x, y, z, m_state, true, m_deleteRemoved);
                //    m_deleteRemoved++;
                //    removedLastTime = true;
                //}
                //else if (!meshDeleted(ptr))
                //    m_matrices[ptr++] = *getMesh(x, y, z, m_state, false, 0);
                //else
                //    std::cout << "deleted ptr, skip: " << ptr - 1 << std::endl;
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
                render(x, y, z, m_state);
            }
        }
    }

    deactivate();
}

void Repeater::drawInstanced()
{
    activate();
    int deletedCount = 0;
    if (m_state->deleted)
        deletedCount = m_state->deleted->size();
    glDrawElementsInstanced(GL_TRIANGLES, m_indiceCount, GL_UNSIGNED_INT, 0, getObjCount());
    deactivate();
}

void Repeater::setInstanced(bool instanced)
{
    m_state->instanced = instanced;
}
        
void Repeater::draw()
{
    if (m_state->instanced)
        drawInstanced();
    else
        drawNonInstanced();
}

