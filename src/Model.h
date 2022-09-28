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
#include "Physics.h"
#include "MousePicker.h"
#include "Camera.h"

class Model
{
public:
    Model(Shader* shader, Physics* physics, MousePicker* picker, Camera* camera);

    void LoadModel(const std::string& fileName);
    void RenderModel();
    void ClearModel();

    ~Model();
    std::vector<ModelMesh*>* getMeshList();

    void setInstanced(bool instanced);
    int getObjectCount();
    int getTriangleCount();
    void update();

    // Set model mass for physics
    // Parameter - separate: Loop through all the meshes and set their mass, otherwise set mass for only the first mesh
    void setMass(float mass, bool separate);

    // Get the count of physics object count from all meshes
    int getPhysicsObjectCount();

private:

    void LoadNode(aiNode* node, const aiScene* scene);
    void LoadMesh(aiMesh* mesh, const aiScene* scene);
    void LoadMaterials(const aiScene* scene);

    std::vector<ModelMesh*> meshList;
    std::vector<Texture*> textureList;
    std::vector<unsigned int> meshToTex;

    // Objects
    Shader* m_shader;
    Physics* m_physics;
    MousePicker* m_mousePicker;
    Camera* m_camera;
};

