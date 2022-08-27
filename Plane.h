#ifndef PLANE_H
#define PLANE_H

#include "Mesh.h"
#include <vector>

class Plane : public Mesh
{
public:
    Plane(Shader* shader);
    ~Plane();

    void draw();
    void drawInstanced();
    void update(int rows, int columns, float scale);
    void createBuffer();

private:
    Shader* m_shader;
    int m_rows;
    int m_columns;
    float m_scale;

    unsigned int m_buffer;
    glm::mat4* m_matrices;
};

#endif PLANE_H