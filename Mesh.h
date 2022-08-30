#ifndef MESH_H
#define MESH_H

#include "RepeaterState.h"
#include "Shader.h"


class Mesh
{

public:
    Mesh(Shader* shader,
        float* vertices,
        unsigned int* indices,
        unsigned int verticeCount,
        unsigned int indiceCount
    );
    ~Mesh();

    //void render(
    //    int xPos, int yPos, int zPos,
    //    float scale, float padding,
    //    float xOffset, float yOffset, float zOffset,
    //    float angle, float xRotation, float yRotation, float zRotation
    //);

    void render(int xPos, int yPos, int zPos, RepeaterState* state);

    //glm::mat4* getMesh(
    //    int xPos, int yPos, int zPos,
    //    float scale, float padding,
    //    float xOffset, float yOffset, float zOffset,
    //    float angle, float xRotation, float yRotation, float zRotation
    //);

    glm::mat4* getMesh(int xPos, int yPos, int zPos, RepeaterState* state);

    void setTexture1(unsigned int& texture);
    void setTexture2(unsigned int& texture);

    void setShader(Shader* shader);

    void activate();
    void deactivate();

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

    unsigned int m_triangleCount;

    void create();
    
};

#endif // MESH_H