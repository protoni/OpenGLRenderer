#ifndef MODEL_MESH_H
#define MODEL_MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include <vector>
#include <string>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct ModelTexture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class ModelMesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<ModelTexture> textures;

    ModelMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<ModelTexture> textures);

    void draw(Shader &shader);

private:
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

#endif // MODEL_MESH_H