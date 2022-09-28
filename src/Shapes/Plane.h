#ifndef PLANE_H
#define PLANE_H

#include "../Repeater.h"

class Plane : public Repeater
{
public:
    Plane(Shader* shader, bool instanced, bool isLight = false, bool useNormals = false);
    ~Plane();
};

#endif // PLANE_H