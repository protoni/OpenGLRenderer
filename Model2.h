#pragma once

#include <vector>
#include <string>

#include <glad_opengl4_3/glad.h>
#include <GLFW/glfw3.h>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "ModelMesh2.h"
#include "Texture.h"

class Model2
{
public:
    Model2();

    void LoadModel(const std::string& fileName);
    void RenderModel();
    void ClearModel();

    ~Model2();

private:

    void LoadNode(aiNode* node, const aiScene* scene);
    void LoadMesh(aiMesh* mesh, const aiScene* scene);
    void LoadMaterials(const aiScene* scene);

    std::vector<ModelMesh2*> meshList;
    std::vector<Texture*> textureList;
    std::vector<unsigned int> meshToTex;
};

