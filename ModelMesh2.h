#pragma once

#include <glad_opengl4_3/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ModelMesh2
{

public:
    ModelMesh2();

    void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void RenderMesh();
    void ClearMesh();

    ~ModelMesh2();

private:
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;
};

