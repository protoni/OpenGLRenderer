#include "Custom.h"

#include "../DebugMacros.h"

float test_vertices[] = {
    // Location         // Texture coords
    0.0

};

unsigned int test_indices[] = {
    0
};

Custom::Custom(Shader* shader, bool instanced, bool isLight, bool useNormals)
    : Repeater(shader,
        instanced,
        test_vertices,
        test_indices,
        sizeof(test_vertices) / sizeof(test_vertices[0]),
        sizeof(test_indices) / sizeof(test_indices[0]),
        isLight,
        useNormals)
{

    m_indices = new std::vector<unsigned int>;
    m_interleavedVertices = new std::vector<float>;
    init();
    setIndiceCount(m_indices->size());
    //create();
    createVBO(m_indices, m_interleavedVertices);
    
    std::cout << "Custom object created!" << std::endl;
}

Custom::~Custom()
{
    std::cout << "Custom destructor called!" << std::endl;
    if (m_indices) {
        delete m_indices;
        m_indices = NULL;
    }

    if (m_interleavedVertices) {
        delete m_interleavedVertices;
        m_interleavedVertices = NULL;
    }
}

void Custom::addVertex(float x, float y, float z)
{
    m_vertices.push_back(x);
    m_vertices.push_back(y);
    m_vertices.push_back(z);
}

void Custom::addNormal(float nx, float ny, float nz)
{
    m_normals.push_back(nx);
    m_normals.push_back(ny);
    m_normals.push_back(nz);
}

void Custom::addTexCoord(float s, float t)
{
    m_texCoords.push_back(s);
    m_texCoords.push_back(t);
}

void Custom::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    m_indices->push_back(i1);
    m_indices->push_back(i2);
    m_indices->push_back(i3);
}

void Custom::clearArrays()
{
    std::vector<float>().swap(m_vertices);
    std::vector<float>().swap(m_normals);
    std::vector<float>().swap(m_texCoords);
    std::vector<unsigned int>().swap(*m_indices);
    std::vector<unsigned int>().swap(m_lineIndices);
}

/*
float triangle_vertices[] = {
    // Location         // Texture coords
    -0.5f, 0.0f,  0.5f,  0.0f, 0.0f, // far left
     0.5f, 0.0f,  0.5f,  0.0f, 0.0f, // far rigt
     0.5f, 0.0f, -0.5f,  0.0f, 0.0f  // near right

};

unsigned int triangle_indices[] = {
    0, 1, 2
};*/
void Custom::init()
{
    // Vertices
    m_interleavedVertices->push_back(-0.5f);
    m_interleavedVertices->push_back(0.0f);
    m_interleavedVertices->push_back(0.5f);
    m_interleavedVertices->push_back(0.0f);
    m_interleavedVertices->push_back(0.0f);


    m_interleavedVertices->push_back(0.5f);
    m_interleavedVertices->push_back(0.0f);
    m_interleavedVertices->push_back(0.5f);
    m_interleavedVertices->push_back(0.0f);
    m_interleavedVertices->push_back(0.0f);


    m_interleavedVertices->push_back(0.5f);
    m_interleavedVertices->push_back(0.0f);
    m_interleavedVertices->push_back(-0.5f);
    m_interleavedVertices->push_back(0.0f);
    m_interleavedVertices->push_back(0.0f);

    // -0.5f, 0.0f, -0.5f->  0.0f, 0.0f,
    m_interleavedVertices->push_back(-0.5f);
    m_interleavedVertices->push_back(0.0f);
    m_interleavedVertices->push_back(-0.5f);
    m_interleavedVertices->push_back(0.0f);
    m_interleavedVertices->push_back(0.0f);

    // Indices
    m_indices->push_back(0);
    m_indices->push_back(1);
    m_indices->push_back(2);
    m_indices->push_back(2);
    m_indices->push_back(3);
    m_indices->push_back(0);
}