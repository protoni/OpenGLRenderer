#include "Mesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "DebugMacros.h"



Mesh::Mesh(Shader* shader, float* vertices, unsigned int* indices, unsigned int vertexCount, unsigned int indiceCount) :
    m_shader(shader), m_vertices(vertices), m_indices(indices), 
    m_vertexCount(vertexCount), m_indiceCount(indiceCount),
    m_VAO(0), m_VBO(0), m_EBO(0), m_texture1(-1), m_texture2(-1), m_useVectors(false)
{
    create();
}

Mesh::Mesh(Shader* shader) : m_VAO(0), m_VBO(0), m_EBO(0), m_texture1(-1), m_texture2(-1), m_useVectors(false), m_vertices(), m_indices()
{
}

Mesh::~Mesh()
{
    cleanup();
}


void Mesh::create()
{
    glGenVertexArrays(1, &m_VAO);

    // Create Buffers
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    // Use vertex array
    glBindVertexArray(m_VAO);

    // hexagon
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_vertexCount, m_vertices, GL_STATIC_DRAW);

    // hexagon
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices[0]) * m_indiceCount, m_indices, GL_STATIC_DRAW);

    // Link vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::createVBO(std::vector<unsigned int>* indices, std::vector<float>* vertices)
{
    if (indices->size() < 1 || vertices->size() < 1) {
        std::cout << "CreateVBO error! Empty vector." << std::endl;
        std::cout << "vertices: " << vertices->size() << ", indices: " << indices->size() << std::endl;
        return;
    }

    m_interleavedVertices = vertices;
    m_indicesVec = indices;
    m_indiceCount = indices->size();
    m_useVectors = true;
    
    glGenVertexArrays(1, &m_VAO);
    
    // Create Buffers
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    // Use vertex array
    glBindVertexArray(m_VAO);
    
    std::cout << "m_interleavedVertices: " << vertices->size() << ", indices: " << indices->size() << std::endl;
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices->at(0)) * vertices->size(), &vertices->front(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices->at(0)) * indices->size(), &indices->front(), GL_STATIC_DRAW);
    
    // Link vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Mesh::activate()
{
    //glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
}

void Mesh::deactivate()
{
    glBindVertexArray(0);
}

void Mesh::setShader(Shader* shader)
{
    m_shader = shader;
}

void Mesh::render(int xPos, int yPos, int zPos, RepeaterState* state)
{
    if (xPos < 1)
        xPos = 1;
    if (yPos < 1)
        yPos = 1;
    if (zPos < 1)
        zPos = 1;

    glm::mat4 model = *getMesh(xPos, yPos, zPos, state, 0, 0);

    m_shader->setMat4("model", model);

    glDrawElements(GL_TRIANGLES, m_indiceCount, GL_UNSIGNED_INT, 0);
}

glm::mat4* Mesh::getMesh(int xPos, int yPos, int zPos, RepeaterState* state, bool meshDeleted, unsigned int deletedRemoved)
{
    if (!m_shader || !m_VAO || !m_EBO) {
        std::cout << "getMesh error!" << std::endl;
        return NULL;
    }

    glm::mat4 model = glm::mat4(1.0f);

    
    if (!meshDeleted) {
    //    std::cout << "Mesh deleted!" << std::endl;
    //    model = glm::translate(model, glm::vec3(
    //        (((0.5f * state->scaleX) * xPos) + (state->paddingX * xPos) + (state->xOffset * 2)),
    //        (((0.5f * state->scaleY) * yPos) + (state->paddingY * yPos) + (state->yOffset)),
    //        (((0.5f * state->scaleZ) * zPos) + (state->paddingZ * zPos) + (state->zOffset)))
    //    );
    //}
    //else {
        model = glm::translate(model, glm::vec3(
            ((0.5f * state->scaleX) * xPos) + (state->paddingX * xPos) + state->xOffset,
            ((0.5f * state->scaleY) * yPos) + (state->paddingY * yPos) + state->yOffset,
            ((0.5f * state->scaleZ) * zPos) + (state->paddingZ * zPos) + state->zOffset)
        );
    }

    if (state->scaleX != 0 && state->scaleY != 0 && state->scaleZ != 0 &&
        state->xRotation != 0 && state->yRotation != 0 && state->zRotation != 0) {
        model = glm::rotate(model, glm::radians(state->angle), glm::vec3(state->xRotation, state->yRotation, state->zRotation));
    }
    model = glm::scale(model, glm::vec3(0.5f * state->scaleX, 0.5f * state->scaleY, 0.5f * state->scaleZ));
    

    return &model;
}

void Mesh::setTexture1(unsigned int& texture)
{
    m_texture1 = texture;
}

void Mesh::setTexture2(unsigned int& texture)
{
    m_texture2 = texture;
}

unsigned int Mesh::getIndexCount()
{
    return m_indiceCount;
}

void Mesh::cleanup()
{
    if (m_useVectors) {
        if (m_indicesVec) {
            delete m_indicesVec;
            m_indicesVec = NULL;
        }

        if (m_interleavedVertices) {
            delete m_interleavedVertices;
            m_interleavedVertices = NULL;
        }
    }
}