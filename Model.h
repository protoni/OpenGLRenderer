#ifndef MODEL_H
#define MODEL_H

#include "ModelMesh.h"

#include "stb_image.h"

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int TextureFromFile(const std::string& path, const std::string& directory, bool gamma = false);

class Model
{
public:
    Model(const std::string& path);
    ~Model();

    void draw(Shader& shader);
private:
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    ModelMesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<ModelTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    std::vector<ModelMesh> m_meshes;
    std::string m_directory;

    // Store all loaded textures
    std::vector<ModelTexture> m_textures_loaded;
};

#endif // MODEL_H