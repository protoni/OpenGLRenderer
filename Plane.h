#ifndef PLANE_H
#define PLANE_H

#include "Mesh.h"
#include <vector>

class Plane : public Mesh
{
public:
    Plane(Shader* shader, bool instanced);
    Plane(Shader* shader, bool instanced, float* vertices, unsigned int* indices, unsigned int verticeCount, unsigned int indiceCount);
    ~Plane();

    void drawNonInstanced();
    void drawInstanced(int faceCount = 0);
    void update(int rows, int columns, int stacks, float scale, float padding);
    void createBuffer();

private:
    Shader* m_shader;
    int m_rows;
    int m_columns;
    int m_stacks;
    float m_scale;
    float m_padding;
    bool m_instanced;

    unsigned int m_buffer;
    glm::mat4* m_matrices;
};

#endif PLANE_H