#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../Repeater.h"

class Triangle : public Repeater
{
public:
    Triangle(Shader* shader, bool instanced);
    ~Triangle();
};

#endif // TRIANGLE_H