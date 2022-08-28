#ifndef CUBE_H
#define CUBE_H

#include "Plane.h"

class Cube : public Plane
{
public:
    Cube(Shader* shader, bool instanced);
    ~Cube();
};

#endif // CUBE_H