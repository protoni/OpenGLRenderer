#ifndef REPEATER_H
#define REPEATER_H

#include "Mesh.h"
#include "RepeaterState.h"

#include <vector>



class Repeater : public Mesh
{
public:
    //Repeater(Shader* shader, bool instanced);
    Repeater(Shader* shader, bool instanced, float* vertices, unsigned int* indices, unsigned int verticeCount, unsigned int indiceCount);
    ~Repeater();

    void drawNonInstanced();
    void drawInstanced();
    void draw();

    void update();
    void createBuffer();
    int getObjCount();
    RepeaterState* getState();

    void setInstanced(bool instanced);

private:
    Shader* m_shader;
    RepeaterState* m_state;

    unsigned int m_indiceCount;
    unsigned int m_buffer;
    glm::mat4* m_matrices;
};

#endif // REPEATER_H