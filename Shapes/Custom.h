#ifndef CUSTOM_H
#define CUSTOM_H

#include "../Repeater.h"

/*
Custom object mesh class for testing purposes
*/

class Custom : public Repeater
{
public:
    Custom(Shader* shader, bool instanced, bool isLight = false, bool useNormals = false);
    ~Custom();



private:
    void addVertex(float x, float y, float z);
    void addNormal(float nx, float ny, float nz);
    void addTexCoord(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
 
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

    std::vector<unsigned int>* m_indices;
    std::vector<float>* m_interleavedVertices;

    std::vector<unsigned int> m_lineIndices;

    
};

#endif // CUSTOM_H