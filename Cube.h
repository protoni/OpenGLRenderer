#ifndef CUBE_H
#define CUBE_H

#include "Repeater.h"

class Cube : public Repeater
{
public:
    Cube(Shader* shader, bool instanced);
    ~Cube();
};

#endif // CUBE_H