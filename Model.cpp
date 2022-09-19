#include "Model.h"

#include <iostream>

Model::Model(Shader* shader) : m_shader(shader)
{
}

Model::~Model()
{
    ClearModel();
}

void Model::RenderModel()
{
    for (size_t i = 0; i < meshList.size(); i++)
    {
        unsigned int materialIndex = meshToTex[i];

        if (materialIndex < textureList.size() && textureList[materialIndex])
        {
            textureList[materialIndex]->use(0);
        }

        //meshList[i]->RenderMesh();
        meshList[i]->drawNonInstanced();
        
        Texture::deactivate();
    }
}

void Model::LoadModel(const std::string& fileName)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        fileName,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        //aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_OptimizeMeshes
    );

    if (!scene)
    {
        printf("Model (%s) failed to load: %s", fileName, importer.GetErrorString());
        return;
    }

    LoadNode(scene->mRootNode, scene);

    LoadMaterials(scene);
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }

    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        LoadNode(node->mChildren[i], scene);
    }
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
        if (mesh->mTextureCoords[0])
        {
            vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
        }
        else {
            vertices.insert(vertices.end(), { 0.0f, 0.0f });
        }
        vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    ModelMesh* newMesh = new ModelMesh(m_shader);
    newMesh->CreateMesh(vertices, indices, vertices.size(), indices.size());
    meshList.push_back(newMesh);
    meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
    textureList.resize(scene->mNumMaterials);

    for (size_t i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* material = scene->mMaterials[i];

        textureList[i] = nullptr;

        if (material->GetTextureCount(aiTextureType_DIFFUSE))
        {
            aiString path;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            {
                int idx = std::string(path.data).rfind("\\");
                std::string filename = std::string(path.data).substr(idx + 1);

                std::string texPath = std::string("Textures/") + filename;

                textureList[i] = new Texture(texPath.c_str(), true);

                std::cout << "Loading file:" << texPath << std::endl;
                if (!textureList[i]->load())
                {
                    printf("Failed to load texture at: %s\n", texPath);
                    delete textureList[i];
                    textureList[i] = nullptr;
                }
                else {
                    std::cout << "Loaded texture: " << filename << std::endl;
                    //printf("Loaded texture: %s \n", filename);
                }
            }
        }

        if (!textureList[i])
        {
            std::cout << "Loading file: Textures/plain.png" << std::endl;
            textureList[i] = new Texture("Textures/plain.png");
            textureList[i]->load();
        }
    }
}

void Model::ClearModel()
{
    std::cout << "Clearing model!" << std::endl;
    for (size_t i = 0; i < meshList.size(); i++)
    {
        if (meshList[i])
        {
            delete meshList[i];
            meshList[i] = nullptr;
        }
    }

    for (size_t i = 0; i < textureList.size(); i++)
    {
        if (textureList[i])
        {
            delete textureList[i];
            textureList[i] = nullptr;
        }
    }
}

std::vector<ModelMesh*>* Model::getMeshList()
{
    return &meshList;
}

void Model::setInstanced(bool instanced)
{
    for (size_t i = 0; i < meshList.size(); i++)
    {
        meshList.at(i)->setInstanced(instanced);
    }
}

int Model::getObjectCount()
{
    return meshList.at(0)->getObjCount();
}

int Model::getTriangleCount()
{
    return meshList.at(0)->getObjCount() * meshList.at(0)->getIndexCount();// * meshList.size()) * (meshList.at(0)->getIndexCount() * meshList.size());
}

void Model::update()
{
    for (size_t i = 0; i < meshList.size(); i++)
    {
        if (i > 0)
            meshList.at(i)->getState()->columnCount = meshList.at(0)->getState()->columnCount;
            meshList.at(i)->getState()->rowCount = meshList.at(0)->getState()->rowCount;
            meshList.at(i)->getState()->stackCount = meshList.at(0)->getState()->stackCount;

            meshList.at(i)->getState()->transformations->paddingX = meshList.at(0)->getState()->transformations->paddingX;
            meshList.at(i)->getState()->transformations->paddingY = meshList.at(0)->getState()->transformations->paddingY;
            meshList.at(i)->getState()->transformations->paddingZ = meshList.at(0)->getState()->transformations->paddingZ;
    }
}
