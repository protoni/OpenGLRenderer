#ifndef MESH_H
#define MESH_H

#include "RepeaterState.h"
#include "Shader.h"
#include <vector>

class Mesh
{

public:
    Mesh(
        Shader* shader,
        float* vertices,
        unsigned int* indices,
        unsigned int verticeCount,
        unsigned int indiceCount,
        bool isLight,
        bool useNormals
    );

    Mesh(
        Shader* shader,
        bool isLight,
        bool useNormals
    );

    ~Mesh();

    void createVBO(std::vector<unsigned int>* indices, std::vector<float>* vertices);
    void createVBO(std::vector<unsigned int>& indices, std::vector<float>& vertices);
    void create();
    void render(int xPos, int yPos, int zPos, RepeaterState* state, unsigned int ptr);

    glm::mat4* getMesh(int xPos, int yPos, int zPos, RepeaterState* state, int ptr);

    void setTexture1(unsigned int& texture);
    void setTexture2(unsigned int& texture);

    unsigned int getIndexCount();

    void setShader(Shader* shader);
    void init();
    void activate();
    void activateLight();
    void deactivate();

    void cleanup();

    bool getIsLight() { return m_isLight; }

private:
    Shader* m_shader;

    float* m_vertices;

    unsigned int* m_indices;
    unsigned int m_vertexCount;
    unsigned int m_indiceCount;

    unsigned int m_VAO;
    unsigned int m_lightVAO;
    unsigned int m_VBO;
    unsigned int m_EBO;

    unsigned int m_texture1;
    unsigned int m_texture2;

    unsigned int m_triangleCount;

    
    
    //std::vector<unsigned int> m_indicesVec;
    //std::vector<float> m_interleavedVertices;

    bool m_useVectors;

    std::vector<unsigned int>* m_indicesVec;
    std::vector<float>* m_interleavedVertices;

    bool m_isLight;
    bool m_useNormals;

};

#endif // MESH_H