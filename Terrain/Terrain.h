#ifndef TERRAIN_H
#define TERRAIN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Terrain
{
public:
    float SIZE = 10.0f;
    int VERTEX_COUNT = 128;

    Terrain(int gridX, int gridZ);
    ~Terrain();

    // Render the terrain
    void render();

    // Get coordinates
    float getX()
    {
        return m_x;
    }
    float getZ()
    {
        return m_z;
    }

private:

    // Generate terrain data
    bool generateTerrain();

    // Load terrain data in VAOs
    bool loadVAO();

    // Load terrain data in VBOs
    bool loadVBO(std::vector<float> data, unsigned int size, int attrNmbr, int coordSize, int stride);

    // Load index data to VBOs
    bool loadIndices();

    // Terrain data
    //float* m_vertices;
    std::vector<float> m_vertices;
    std::vector<float> m_normals;
    std::vector<float> m_texCoords;
    //float* m_normals;
    //float* m_texCoords;
    //int*   m_indices;
    std::vector<int> m_indices;
    //unsigned int m_indexCount;

    // Buffer objects
    std::vector<unsigned int> m_VBOs;
    unsigned int m_VAO;

    float m_x;
    float m_z;
};

#endif // TERRAIN_H