#include "Sphere.h"

#include "../DebugMacros.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// Dummy vertices and indices because can't figure out why model matrix setting fails if these arent passed on to base class ( Mesh )
float sphere_vertices[] = {
    // Location         // Texture coords
    0.0

};

unsigned int sphere_indices[] = {
    0
};

Sphere::Sphere(Shader* shader, bool instanced, bool isLight, bool useNormals)
    : Repeater(shader,
        instanced,
        sphere_vertices,
        sphere_indices,
        sizeof(sphere_vertices) / sizeof(sphere_vertices[0]),
        sizeof(sphere_indices) / sizeof(sphere_indices[0]),
        isLight,
        useNormals)
{

    m_indices = new std::vector<unsigned int>;
    m_interleavedVertices = new std::vector<float>;
    init();
    //buildVerticesSmooth();
    //buildVerticesFlat();
    setIndiceCount(m_indices->size());
    //create();
    createVBO(m_indices, m_interleavedVertices);

    std::cout << "Custom object created!" << std::endl;
}

//Sphere::Sphere(Shader* shader, bool instanced)
//    : Repeater(shader,
//        instanced), m_radius(1.0f), m_sectorCount(36), m_stackCount(18)
//{
//    //buildVerticesSmooth();
//    //buildVerticesFlat();
//    init();
//    setIndiceCount(m_indices.size());
//    createVBO(&m_indices, &m_interleavedVertices);
//
//    std::cout << "Cube created!" << std::endl;
//}

Sphere::~Sphere()
{
}

void Sphere::buildVerticesSmooth()
{
    const float PI = acos(-1);

    // clear memory of prev arrays
    clearArrays();

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / m_radius;    // normal
    float s, t;                                     // texCoord

    float sectorStep = 2 * PI / m_sectorCount;
    float stackStep = PI / m_stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= m_stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = m_radius * cosf(stackAngle);             // r * cos(u)
        z = m_radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= m_sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            addVertex(x, y, z);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            addNormal(nx, ny, nz);

            // vertex tex coord between [0, 1]
            s = (float)j / m_sectorCount;
            t = (float)i / m_stackCount;
            addTexCoord(s, t);
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for (int i = 0; i < m_stackCount; ++i)
    {
        k1 = i * (m_sectorCount + 1);     // beginning of current stack
        k2 = k1 + m_sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < m_sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0)
            {
                addIndices(k1, k2, k1 + 1);   // k1---k2---k1+1
            }

            if (i != (m_stackCount - 1))
            {
                addIndices(k1 + 1, k2, k2 + 1); // k1+1---k2---k2+1
            }

            // vertical lines for all stacks
            m_lineIndices.push_back(k1);
            m_lineIndices.push_back(k2);
            if (i != 0)  // horizontal lines except 1st stack
            {
                m_lineIndices.push_back(k1);
                m_lineIndices.push_back(k1 + 1);
            }
        }
    }

    // generate interleaved vertex array as well
    buildInterleavedVertices();
}

void Sphere::buildVerticesFlat()
{
    const float PI = acos(-1);

    // tmp vertex definition (x,y,z,s,t)
    struct Vertex
    {
        float x, y, z, s, t;
    };
    std::vector<Vertex> tmpVertices;

    float sectorStep = 2 * PI / m_sectorCount;
    float stackStep = PI / m_stackCount;
    float sectorAngle, stackAngle;

    // compute all vertices first, each vertex contains (x,y,z,s,t) except normal
    for (int i = 0; i <= m_stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        float xy = m_radius * cosf(stackAngle);       // r * cos(u)
        float z = m_radius * sinf(stackAngle);        // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= m_sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            Vertex vertex;
            vertex.x = xy * cosf(sectorAngle);      // x = r * cos(u) * cos(v)
            vertex.y = xy * sinf(sectorAngle);      // y = r * cos(u) * sin(v)
            vertex.z = z;                           // z = r * sin(u)
            vertex.s = (float)j / m_sectorCount;        // s
            vertex.t = (float)i / m_stackCount;         // t
            tmpVertices.push_back(vertex);
        }
    }

    // clear memory of prev arrays
    clearArrays();

    Vertex v1, v2, v3, v4;                          // 4 vertex positions and tex coords
    std::vector<float> n;                           // 1 face normal

    int i, j, k, vi1, vi2;
    int index = 0;                                  // index for vertex
    for (i = 0; i < m_stackCount; ++i)
    {
        vi1 = i * (m_sectorCount + 1);                // index of tmpVertices
        vi2 = (i + 1) * (m_sectorCount + 1);

        for (j = 0; j < m_sectorCount; ++j, ++vi1, ++vi2)
        {
            // get 4 vertices per sector
            //  v1--v3
            //  |    |
            //  v2--v4
            v1 = tmpVertices[vi1];
            v2 = tmpVertices[vi2];
            v3 = tmpVertices[vi1 + 1];
            v4 = tmpVertices[vi2 + 1];

            // if 1st stack and last stack, store only 1 triangle per sector
            // otherwise, store 2 triangles (quad) per sector
            if (i == 0) // a triangle for first stack ==========================
            {
                // put a triangle
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v4.x, v4.y, v4.z);

                // put tex coords of triangle
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v4.s, v4.t);

                // put normal
                n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);
                for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put indices of 1 triangle
                addIndices(index, index + 1, index + 2);

                // indices for line (first stack requires only vertical line)
                m_lineIndices.push_back(index);
                m_lineIndices.push_back(index + 1);

                index += 3;     // for next
            }
            else if (i == (m_stackCount - 1)) // a triangle for last stack =========
            {
                // put a triangle
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);

                // put tex coords of triangle
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v3.s, v3.t);

                // put normal
                n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put indices of 1 triangle
                addIndices(index, index + 1, index + 2);

                // indices for lines (last stack requires both vert/hori lines)
                m_lineIndices.push_back(index);
                m_lineIndices.push_back(index + 1);
                m_lineIndices.push_back(index);
                m_lineIndices.push_back(index + 2);

                index += 3;     // for next
            }
            else // 2 triangles for others ====================================
            {
                // put quad vertices: v1-v2-v3-v4
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);
                addVertex(v4.x, v4.y, v4.z);

                // put tex coords of quad
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v3.s, v3.t);
                addTexCoord(v4.s, v4.t);

                // put normal
                n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                for (k = 0; k < 4; ++k)  // same normals for 4 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put indices of quad (2 triangles)
                addIndices(index, index + 1, index + 2);
                addIndices(index + 2, index + 1, index + 3);

                // indices for lines
                m_lineIndices.push_back(index);
                m_lineIndices.push_back(index + 1);
                m_lineIndices.push_back(index);
                m_lineIndices.push_back(index + 2);

                index += 4;     // for next
            }
        }
    }

    // generate interleaved vertex array as well
    buildInterleavedVertices();
}

void Sphere::addVertex(float x, float y, float z)
{
    //m_vertices.push_back(x);
    //m_vertices.push_back(y);
    //m_vertices.push_back(z);

    m_interleavedVertices->push_back(x);
    m_interleavedVertices->push_back(y);
    m_interleavedVertices->push_back(z);
}

void Sphere::addNormal(float nx, float ny, float nz)
{
    m_normals.push_back(nx);
    m_normals.push_back(ny);
    m_normals.push_back(nz);
}

void Sphere::addTexCoord(float s, float t)
{
    m_texCoords.push_back(s);
    m_texCoords.push_back(t);
}

void Sphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    m_indices->push_back(i1);
    m_indices->push_back(i2);
    m_indices->push_back(i3);
}

void Sphere::buildInterleavedVertices()
{
    // Clear
    std::vector<float>().swap(*m_interleavedVertices);

    std::size_t i, j;
    std::size_t count = m_vertices.size();
    for (i = 0; i < count; i += 3)
    {
        m_interleavedVertices->push_back(m_vertices[i]);
        m_interleavedVertices->push_back(m_vertices[i + 1]);
        m_interleavedVertices->push_back(m_vertices[i + 2]);

        //m_interleavedVertices.push_back(m_normals[i]);
        //m_interleavedVertices.push_back(m_normals[i + 1]);
        //m_interleavedVertices.push_back(m_normals[i + 2]);
        //
        //m_interleavedVertices.push_back(m_texCoords[j]);
        //m_interleavedVertices.push_back(m_texCoords[j + 1]);
    }
}

void Sphere::clearArrays()
{
    std::vector<float>().swap(m_vertices);
    std::vector<float>().swap(m_normals);
    std::vector<float>().swap(m_texCoords);
    std::vector<unsigned int>().swap(*m_indices);
    std::vector<unsigned int>().swap(m_lineIndices);
}

std::vector<float> Sphere::computeFaceNormal(float x1, float y1, float z1,  // v1
    float x2, float y2, float z2,  // v2
    float x3, float y3, float z3)  // v3
{
    const float EPSILON = 0.000001f;

    std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
    float nx, ny, nz;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;

    // cross product: e1 x e2
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if (length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;
    }

    return normal;
}

void Sphere::init()
{
    int i, j;
    //std::vector<GLfloat> vertices;
    //std::vector<GLuint> indices;
    int lats = 80;
    int longs = 20;
    int indicator = 0;
    for (i = 0; i <= lats; i++) {
        double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = glm::pi<double>() * (-0.5 + (double)i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        for (j = 0; j <= longs; j++) {
            double lng = 2 * glm::pi<double>() * (double)(j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            m_interleavedVertices->push_back(x * zr0);
            m_interleavedVertices->push_back(y * zr0);
            m_interleavedVertices->push_back(z0);
            m_indices->push_back(indicator);
            indicator++;

            m_interleavedVertices->push_back(x * zr1);
            m_interleavedVertices->push_back(y * zr1);
            m_interleavedVertices->push_back(z1);
            m_indices->push_back(indicator);
            indicator++;
        }
        //indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    }
}