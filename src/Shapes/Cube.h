#ifndef CUBE_H
#define CUBE_H

#include "../Repeater.h"

class Cube : public Repeater
{
public:
    Cube(Shader* shader, bool instanced, bool isLight = false, bool useNormals = false);
    ~Cube();
};

#endif // CUBE_H