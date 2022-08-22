#ifndef MESH_H
#define MESH_H

#include "Shader.h"

class Mesh
{

public:
    Mesh(Shader* shader, float* vertices, unsigned int* indices, unsigned int verticeCount, unsigned int indiceCount);
    ~Mesh();

    void render(int& faceCounter);
    void setTexture1(unsigned int& texture);
    void setTexture2(unsigned int& texture);

private:
    Shader* m_shader;

    float* m_vertices;

    unsigned int* m_indices;
    unsigned int m_vertexCount;
    unsigned int m_indiceCount;

    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_EBO;

    unsigned int m_texture1;
    unsigned int m_texture2;

    void create();
    
};

#endif // MESH_H