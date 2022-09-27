#ifndef TERRAIN_H
#define TERRAIN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "../Texture.h"

class Terrain
{
public:
    float SIZE = 200.0f;
    float MAX_HEIGHT = 10.0f;
    float MAX_PIXEL_COLOR = 256 * 256 * 256;
    int VERTEX_COUNT = 128;

    Terrain(int gridX, int gridZ);
    ~Terrain();

    // Render the terrain
    void render();

    // Get coordinates
    float getX() { return m_x; }
    float getZ() { return m_z; }

    // Get indice count
    int getIndiceCount() { return m_indices.size(); }

private:

    // Generate terrain data
    bool generateTerrain();

    // Load terrain data in VAOs
    bool loadVAO();

    // Load terrain data in VBOs
    bool loadVBO(std::vector<float> data, unsigned int size, int attrNmbr, int coordSize, int stride);

    // Load index data to VBOs
    bool loadIndices();

    // Read heightmap pixel value and get the height
    float getVertexYpos(int x, int z);

    // Terrain data
    std::vector<float> m_vertices;
    std::vector<float> m_normals;
    std::vector<float> m_texCoords;
    std::vector<int> m_indices;

    // Textures
    Texture* m_terrainTexture;
    Texture* m_terrainHeightMap;

    // Buffer objects
    std::vector<unsigned int> m_VBOs;
    unsigned int m_VAO;

    float m_x;
    float m_z;
};

#endif // TERRAIN_H