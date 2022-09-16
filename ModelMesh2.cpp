#include "ModelMesh2.h"

float model_vertices[] = {
    // Location         // Texture coords
    0.0

};

unsigned int model_indices[] = {
    0
};

ModelMesh2::ModelMesh2(Shader* shader)
    : Repeater(shader,
    false,
        model_vertices,
        model_indices,
    sizeof(model_vertices) / sizeof(model_vertices[0]),
    sizeof(model_indices) / sizeof(model_indices[0]),
    false,
    false)
{
    VAO = 0;
    VBO = 0;
    IBO = 0;
    indexCount = 0;
}

//void ModelMesh2::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
void ModelMesh2::CreateMesh(std::vector<GLfloat>& vertices, std::vector<unsigned int>& indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
    setIndiceCount(indices.size());
    //create();
    createVBO(indices, vertices);

    //indexCount = numOfIndices;
    //
    ///*
    //Create VAO, This creates a vertex in the graphics cards memory.
    //params:
    //0: amount of VAOs to create
    //1: where to store the ID of the VAO */
    //glGenVertexArrays(1, &VAO);
    //glBindVertexArray(VAO);
    //
    //glGenBuffers(1, &IBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);
    //
    //// Create buffer object
    //glGenBuffers(1, &VBO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //
    //// Connect the buffer data
    //// GL_STATIC_DRAW = object will not be changed, can be moved around tho
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);
    //
    //// 0 = layout, 3 values at a time ( x, y, z)
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
    //glEnableVertexAttribArray(0);
    //
    //// Normals
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
    //glEnableVertexAttribArray(1);
    //
    //// Texture
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
    //glEnableVertexAttribArray(2);
    //
    //// Unbind
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
}

void ModelMesh2::RenderMesh()
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void ModelMesh2::ClearMesh()
{
    if (IBO != 0)
    {
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }

    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    indexCount = 0;
}

ModelMesh2::~ModelMesh2()
{
    ClearMesh();
}









