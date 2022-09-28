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
    // Load terrain texture
    m_terrainTexture = new Texture("Textures/grass.png", true);

    // Load terrain height map
    m_terrainHeightMap = new Texture("Textures/heightmap.png", true);

    // Generate terrain
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

    if (m_terrainTexture) {
        delete m_terrainTexture;
        m_terrainTexture = NULL;
    }

    if (m_terrainHeightMap) {
        delete m_terrainHeightMap;
        m_terrainHeightMap = NULL;
    }
}

bool Terrain::generateTerrain()
{
    int count = VERTEX_COUNT * VERTEX_COUNT;

    for (int i = 0; i < VERTEX_COUNT; i++) {
        for (int j = 0; j < VERTEX_COUNT; j++) {

            // Calculate terrain position
            unsigned int x = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
            unsigned int z = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
            float y = getVertexYpos(j, i);
            m_vertices.push_back(x);
            m_vertices.push_back(y);
            m_vertices.push_back(z);

            // Calculate terrain normals
            glm::vec3 normal = getNormal(j, i);
            m_normals.push_back(normal.x);
            m_normals.push_back(normal.y);
            m_normals.push_back(normal.z);

            // Calculate terrain texture coordinates
            m_texCoords.push_back((float)j / ((float)VERTEX_COUNT - 1));
            m_texCoords.push_back((float)i / ((float)VERTEX_COUNT - 1));
        }
    }
    

    //glm::mat4* modelMatrices = new glm::mat4[m_indices.size() / 6];
    //for (int i = 0; i < m_indices.size() / 6; i++) {
    //    modelMatrices[i] = glm::mat4(1.0f);
    //}


    for (int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
        for (int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
            int topLeft = (gz * VERTEX_COUNT) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
            int bottomRight = bottomLeft + 1;
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
    m_terrainTexture->use(0);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT * VERTEX_COUNT);
    //glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_indices.size() / 6);
    //std::cout << "m_indexCount: " << m_indices.size() << std::endl;
    glBindVertexArray(0);
}

float Terrain::getVertexYpos(int x, int z)
{
    if (x < 0 || x >= m_terrainHeightMap->getWidth() || z < 0 || z >= m_terrainHeightMap->getHeight())
        return 0.0f;

    float height = m_terrainHeightMap->getRGB(x, z);

    height += MAX_PIXEL_COLOR / 2.0f;
    height /= MAX_PIXEL_COLOR / 2.0f;
    height *= MAX_HEIGHT;
    height -= 20;

    return height;
}

glm::vec3& Terrain::getNormal(int x, int z)
{
    // Get neighboring vertex y values
    float heightL = getVertexYpos(x - 1, z);
    float heightR = getVertexYpos(x + 1, z);
    float heightD = getVertexYpos(x, z - 1);
    float heightU = getVertexYpos(x, z + 1);

    glm::vec3 normal = glm::normalize(glm::vec3(heightL - heightR, 2.0f, heightD - heightU));

    return normal;
}
