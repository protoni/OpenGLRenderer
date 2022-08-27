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
  ), m_rows(1), m_columns(1), m_scale(1.0), m_buffer(0), m_matrices(NULL), m_shader(shader)
{
    // Init instance buffer
    
    
    createBuffer();
}

Plane::~Plane()
{
}

// Create instanced buffer on the GPU
void Plane::createBuffer()
{   
    int ptr = 0;
    
    
    delete[] m_matrices;
    //for (int i = 0; i < m_matrices.size(); i++) {
    //    delete(m_matrices[i]);
    //}
    

    //glBufferData(GL_ARRAY_BUFFER, (m_rows * m_columns) * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
    //std::cout << "Buffered object count: " << sizeof(m_matrices) / sizeof(m_matrices[0]) << std::endl;

    //m_matrices = (glm::mat4*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    m_matrices = new glm::mat4[m_rows * m_columns];
    //m_matrices = new glm::mat4[10];

    //for (int i = 0; i < 10; i++) {
    //    m_matrices[ptr++] = *getMesh(i, 1, m_scale);
    //}

    ptr = 0;
    for (int j = 0; j < m_rows; j++) { // rows
        for (int i = 0; i < m_columns; i++) { // columns
    
            //glm::mat4 model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(((0.5f) * m_scale) * i, 0.0f, 0.0f - (0.5 * m_scale) * j));
            //model = glm::scale(model, glm::vec3(0.5f * m_scale, 0.0f, 0.5f * m_scale));
            //m_shader->setMat4("model", model);
            //getMesh(i, j, m_scale);
            glm::mat4* model = getMesh(i, j, m_scale);
            if (model) {
                m_matrices[ptr++] = *model;
                //m_matrices.push_back(model);
            }
            else
                std::cout << "Error" << std::endl;
        }
    }

    std::cout << "planes: " << m_rows * m_columns << std::endl;

    glGenBuffers(1, &m_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_rows * m_columns * sizeof(glm::mat4), &m_matrices[0], GL_DYNAMIC_DRAW);

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

void Plane::update(int rows, int columns, float scale)
{
    m_rows = rows;
    m_columns = columns;
    m_scale = scale;

    createBuffer();
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

void Plane::drawInstanced()
{
    activate();
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (m_rows * m_columns));
}
