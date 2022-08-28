#ifndef REPEATER_H
#define REPEATER_H

#include "Mesh.h"
#include <vector>

struct RepeaterState
{
    int rowCount;
    int columnCount;
    int stackCount;
    float scale;
    float padding;
    bool instanced;

    RepeaterState() :
        rowCount(1),
        columnCount(1),
        stackCount(1),
        scale(0.5),
        padding(0.0),
        instanced(false) {}
};

class Repeater : public Mesh
{
public:
    //Repeater(Shader* shader, bool instanced);
    Repeater(Shader* shader, bool instanced, float* vertices, unsigned int* indices, unsigned int verticeCount, unsigned int indiceCount);
    ~Repeater();

    void drawNonInstanced();
    void drawInstanced(int faceCount = 1);
    void draw(int faceCount = 1);

    void update();
    void createBuffer();
    int getObjCount();
    RepeaterState* getState();

private:
    Shader* m_shader;
    RepeaterState* m_state;

    unsigned int m_buffer;
    glm::mat4* m_matrices;
};

#endif // REPEATER_H