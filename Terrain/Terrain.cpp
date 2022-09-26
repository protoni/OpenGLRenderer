#include "Terrain.h"



#include <iostream>


float test_verts[] = {
    // Location         // Texture coords
    -0.5f, 0.0f,  0.5f,  0.0f, 0.0f, // far left
     0.5f, 0.0f,  0.5f,  0.0f, 0.0f, // far rigt
     0.5f, 0.0f, -0.5f,  0.0f, 0.0f  // near right

};

unsigned int test_indcs[] = {
    0, 1, 2
};

Terrain::Terrain(int gridX, int gridZ) : m_x(gridX * SIZE), m_z(gridZ * SIZE)
{
    if (!generateTerrain()) {
        std::cout << "Failed to generate terrain!" << std::endl;
    }
}

Terrain::~Terrain()
{
    // Destroy VAO
    glDeleteVertexArrays(1, &m_VAO);

    // Destroy VBOs
    for (int i = 0; i < m_VBOs.size(); i++) {
        glDeleteBuffers(1, &m_VBOs.at(i));
    }

    //if (m_vertices) {
    //    delete[] m_vertices;
    //}

    //if (m_normals) {
    //    delete[] m_normals;
    //}
    //
    //if (m_texCoords) {
    //    delete[] m_texCoords;
    //}

    //if (m_indices) {
    //    delete[] m_indices;
    //}
}

bool Terrain::generateTerrain()
{
    int count = VERTEX_COUNT * VERTEX_COUNT;
    //m_vertices  = new float[count * 3]();
    //m_normals   = new float[count * 3]();
    //m_texCoords = new float[count * 2]();

    //m_indexCount = 6 * ((VERTEX_COUNT - 1) * (VERTEX_COUNT * 1));
    //m_indices = new int[m_indexCount]();
    int vertexPointer = 0;
    //for (int i = 0; i < VERTEX_COUNT; i++) {
    //    for (int j = 0; j < VERTEX_COUNT; j++) {
    //        m_vertices[vertexPointer   * 3     ] = -(float)j / ((float)VERTEX_COUNT - 1) * SIZE;
    //        m_vertices[vertexPointer   * 3 + 1 ] =  0;
    //        m_vertices[vertexPointer   * 3 + 2 ] = -(float)i / ((float)VERTEX_COUNT - 1) * SIZE;
    //        m_normals[vertexPointer   * 3     ] =  0;
    //        m_normals[vertexPointer   * 3 + 1 ] =  1;
    //        m_normals[vertexPointer   * 3 + 2 ] =  0;
    //        m_texCoords[vertexPointer   * 2     ] =  (float)j / ((float)VERTEX_COUNT - 1);
    //        m_texCoords[vertexPointer   * 2 + 1 ] =  (float)i / ((float)VERTEX_COUNT - 1);
    //        vertexPointer++;
    //    }
    //}
    for (int i = 0; i < VERTEX_COUNT; i++) {
        for (int j = 0; j < VERTEX_COUNT; j++) {
            //m_vertices[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
            //m_vertices[(vertexPointer * 3) + 1] = 0;
            //m_vertices[(vertexPointer * 3) + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
            m_vertices.push_back((float)j / ((float)VERTEX_COUNT - 1) * SIZE);
            m_vertices.push_back((rand() % 5) * 0.1);
            m_vertices.push_back((float)i / ((float)VERTEX_COUNT - 1) * SIZE);

            m_normals.push_back(0);
            m_normals.push_back(1);
            m_normals.push_back(0);

            m_texCoords.push_back((float)j / ((float)VERTEX_COUNT - 1));
            m_texCoords.push_back((float)i / ((float)VERTEX_COUNT - 1));

            //m_normals[vertexPointer * 3] = 0;
            //m_normals[vertexPointer * 3 + 1] = 1;
            //m_normals[vertexPointer * 3 + 2] = 0;
            //m_texCoords[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
            //m_texCoords[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
            vertexPointer++;
        }
    }
    

    //glm::mat4* modelMatrices = new glm::mat4[m_indices.size() / 6];
    //for (int i = 0; i < m_indices.size() / 6; i++) {
    //    modelMatrices[i] = glm::mat4(1.0f);
    //}

    int pointer = 0;
    //for (int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
    //    for (int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
    //        int topLeft = (gz * VERTEX_COUNT) + gx;
    //        int topRight = topLeft + 1;
    //        int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
    //        int bottomRight = bottomLeft + 1;
    //        m_indices[pointer++] = topLeft;
    //        m_indices[pointer++] = bottomLeft;
    //        m_indices[pointer++] = topRight;
    //        m_indices[pointer++] = topRight;
    //        m_indices[pointer++] = bottomLeft;
    //        m_indices[pointer++] = bottomRight;
    //    }
    //}

    for (int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
        for (int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
            int topLeft = (gz * VERTEX_COUNT) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
            int bottomRight = bottomLeft + 1;
            //m_indices[pointer++] = topLeft;
            //m_indices[pointer++] = bottomLeft;
            //m_indices[pointer++] = topRight;
            //m_indices[pointer++] = topRight;
            //m_indices[pointer++] = bottomLeft;
            //m_indices[pointer++] = bottomRight;
            m_indices.push_back(topLeft);
            m_indices.push_back(bottomLeft);
            m_indices.push_back(topRight);
            m_indices.push_back(topRight);
            m_indices.push_back(bottomLeft);
            m_indices.push_back(bottomRight);
        }
    }

    // Create vertex array
    glGenVertexArrays(1, &m_VAO);

    // Use vertex array
    glBindVertexArray(m_VAO);

    // Load indices
    loadIndices();

    // Load VBOs
    loadVBO(m_vertices,  count * 3, 0, 3, 0);
    loadVBO(m_texCoords, count * 2, 1, 2, 3);
    loadVBO(m_normals,   count * 3, 2, 3, 5);



    // Load transformation matrices
    //glm::mat4* modelMatrices = new glm::mat4[VERTEX_COUNT * VERTEX_COUNT];

    //unsigned int VBO;
    //glGenBuffers(1, &VBO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    ////glBufferData(GL_ARRAY_BUFFER, SIZE * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, (m_indices.size() / 6) * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
    //
    //glEnableVertexAttribArray(3);
    //glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    //glEnableVertexAttribArray(4);
    //glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
    //glEnableVertexAttribArray(5);
    //glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    //glEnableVertexAttribArray(6);
    //glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    //
    //glVertexAttribDivisor(3, 1);
    //glVertexAttribDivisor(4, 1);
    //glVertexAttribDivisor(5, 1);
    //glVertexAttribDivisor(6, 1);
    //m_VBOs.push_back(VBO);

    // Unbind VAO
    glBindVertexArray(0);

    return true;
}

bool Terrain::loadVBO(std::vector<float> data, unsigned int size, int attrNmbr, int coordSize, int stride)
{
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Load data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, &data[0], GL_STATIC_DRAW);

    // Set VAO coordinates
    glVertexAttribPointer(attrNmbr, coordSize, GL_FLOAT, GL_FALSE, coordSize * sizeof(float), (void*)(stride * sizeof(float)));
    glEnableVertexAttribArray(attrNmbr);

    m_VBOs.push_back(VBO);

    return true;
}

bool Terrain::loadIndices()
{
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(m_indices.at(0)), &m_indices[0], GL_STATIC_DRAW);

    m_VBOs.push_back(VBO);

    return true;
}

void Terrain::render()
{
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT * VERTEX_COUNT);
    //glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_indices.size() / 6);
    std::cout << "m_indexCount: " << m_indices.size() << std::endl;
    glBindVertexArray(0);
}
