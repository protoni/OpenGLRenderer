#pragma once

#include <vector>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Shapes/ModelMesh.h"
#include "Texture.h"
#include "Shader.h"

class Model
{
public:
    Model(Shader* shader);

    void LoadModel(const std::string& fileName);
    void RenderModel();
    void ClearModel();

    ~Model();
    std::vector<ModelMesh*>* getMeshList();

    void setInstanced(bool instanced);
    int getObjectCount();
    int getTriangleCount();
    void update();

private:

    void LoadNode(aiNode* node, const aiScene* scene);
    void LoadMesh(aiMesh* mesh, const aiScene* scene);
    void LoadMaterials(const aiScene* scene);

    std::vector<ModelMesh*> meshList;
    std::vector<Texture*> textureList;
    std::vector<unsigned int> meshToTex;

    Shader* m_shader;
};

