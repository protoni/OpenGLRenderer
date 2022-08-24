#ifndef PLANE_H
#define PLANE_H

#include "Mesh.h"

class Plane : public Mesh
{
public:
    Plane(Shader* shader);
    ~Plane();

    void draw();
    void update(int rows, int columns, float scale);

private:
    int m_rows;
    int m_columns;
    float m_scale;
};

#endif PLANE_H