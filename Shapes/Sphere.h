#ifndef SPHERE_H
#define SPHERE_H

#include "../Repeater.h"

class Sphere : public Repeater
{
public:
    Sphere(Shader* shader, bool instanced);
    ~Sphere();

    void buildVerticesSmooth();
    void buildVerticesFlat();

private:
    void addVertex(float x, float y, float z);
    void addNormal(float nx, float ny, float nz);
    void addTexCoord(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    void buildInterleavedVertices();
    void clearArrays();
    std::vector<float> computeFaceNormal(float x1, float y1, float z1,
        float x2, float y2, float z2, float x3, float y3, float z3);
    void init();


    float m_radius;
    int m_sectorCount;
    int m_stackCount;

    std::vector<float> m_vertices;
    std::vector<float> m_normals;
    std::vector<float> m_texCoords;

    //std::vector<unsigned int> m_indices;
    //std::vector<float> m_interleavedVertices;
    std::vector<unsigned int> m_lineIndices;

    std::vector<unsigned int>* m_indices;
    std::vector<float>* m_interleavedVertices;

    
};

#endif // SPHERE_H