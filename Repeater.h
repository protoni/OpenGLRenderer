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
    Repeater(Shader* shader, bool instanced);
    ~Repeater();

    void drawNonInstanced();
    void drawInstanced();
    void draw();
    void setIndiceCount(unsigned int count);

    void update();
    void createBuffer();
    int getObjCount();
    RepeaterState* getState();

    void setInstanced(bool instanced);

private:
    bool meshDeleted(int meshPointer);

    Shader* m_shader;
    RepeaterState* m_state;

    unsigned int m_indiceCount;
    unsigned int m_buffer;
    glm::mat4* m_matrices;

    unsigned int m_deleteRemoved;

    int m_oldObjectCount;
};

#endif // REPEATER_H