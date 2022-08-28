#include "Mesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh(Shader* shader, float* vertices, unsigned int* indices, unsigned int vertexCount, unsigned int indiceCount) :
    m_shader(shader), m_vertices(vertices), m_indices(indices), 
    m_vertexCount(vertexCount), m_indiceCount(indiceCount),
    m_VAO(0), m_VBO(0), m_EBO(0), m_texture1(-1), m_texture2(-1)
{
    create();
}

Mesh::~Mesh()
{

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

void Mesh::render(
    int xPos, int yPos, int zPos,
    float scale, float padding,
    float xOffset, float yOffset, float zOffset,
    float angle, float xRotation, float yRotation, float zRotation)
{
    glm::mat4 model = *getMesh(
        xPos, yPos, zPos, scale, padding, 
        xOffset, yOffset, zOffset,
        angle, xRotation, yRotation, zRotation);
    m_shader->setMat4("model", model);

    glDrawElements(GL_TRIANGLES, m_indiceCount, GL_UNSIGNED_INT, 0);
}

glm::mat4* Mesh::getMesh(
    int xPos, int yPos, int zPos,
    float scale, float padding,
    float xOffset, float yOffset, float zOffset,
    float angle, float xRotation, float yRotation, float zRotation
)
{
    if (!m_shader || !m_VAO || !m_EBO) {
        std::cout << "getMesh error!" << std::endl;
        return NULL;
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model,glm::vec3(
        ((0.5f * scale) * xPos) + (padding * xPos) + xOffset,
        ((0.5f * scale) * yPos) + (padding * yPos) + yOffset,
        ((0.5f * scale) * zPos) + (padding * zPos) + zOffset)
    );
    
    //model = glm::rotate(model, glm::radians(angle), glm::vec3(xRotation, yRotation, zRotation));
    if (scale != 0 && xRotation != 0 && yRotation != 0 && zRotation != 0) {
        model = glm::rotate(model, glm::radians(angle), glm::vec3(xRotation, yRotation, zRotation));
    }
    model = glm::scale(model, glm::vec3(0.5f * scale, 0.5f * scale, 0.5f * scale));
    //m_shader->setMat4("model", model);

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
