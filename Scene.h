#ifndef SCENE_H
#define SCENE_H

//#include <glad/glad.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Plane.h"
#include "Cube.h"
#include "Window.h"
#include "Repeater.h"
#include "RepeaterState.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Custom.h"
#include "MaterialBase.h"
#include "Model.h"
#include "MeshListHandler.h"
#include "MeshObject.h"
#include "LightHandler.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "windows.h"
#include <crtdbg.h>
#include <thread>

class Scene
{
public:
    Scene(Camera *camera, ScreenSettings* screenSettings);
    ~Scene();

    // Render all objects on the screen currently
    void renderScene();

    // Update the the object's shader based on if it is instanced or not
    void updateMeshShader(bool instanced, int idx);

    // Add different type of objects to the screen
    void addCube();
    void addPlane();
    void addTriangle();
    void addSphere();
    void addCustom();
    void addPointLight();
    void addDirectionalLight();
    void addSpotLight();
    void addReflectingCube();
    void addModel();

    // Add new objects to mesh list
    void addMeshObject(Repeater* mesh, MeshType type);
    
    // Update mesh object state
    bool updateMeshObjects(MeshObjectChange& change);

    // Count all objects in the scene and return the count
    int getObjectCount();

    // Count the total amount of triangles of the meshes on the Scene currently and return the count
    int getTriangleCount();

    // Return all objects on the screen
    std::vector<MeshObject*>* getMeshList();

    // Delete mesh object
    void deleteObject(int idx);

    // Delete model object
    void deleteModel(int idx);

    // Clean up scene by deleting all objects
    void clean();

    // Update mesh's material to predefined one
    void updateMeshMaterial(int selected, const std::string& newMaterial);

    // Get the current mesh pointer index from MeshListHandler
    int getMeshPointer();

private:

    // Render a single mesh object
    void draw(int idx, glm::mat4& projection, glm::mat4& view);

    // Render a single model object
    void drawModel(int idx, glm::mat4& projection, glm::mat4& view);

    // Class objects
    Camera* m_camera;
    Shader* m_ourShader;
    Shader* m_lightShader;
    Shader* m_lightMeshShader;
    Shader* m_ourShaderInstanced;
    Shader* m_modelLoadingShader;
    Texture* m_container_texture;
    Texture* m_container_texture_specular;
    ScreenSettings* m_screenSettings;
    MeshListHandler* m_meshListHandler;
    MaterialHandler* m_materialHandler;
    LightHandler* m_lightHandler;

    // All of the meshes currently in the scene
    std::vector<MeshObject*>* m_meshList;

    // All of the lights present in the scene
    std::vector<MeshObject*> m_pointLights;
    std::vector<MeshObject*> m_directionalLights;
    std::vector<MeshObject*> m_spotLights;
};

#endif // SCENE_H