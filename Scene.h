#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Plane.h"
#include "Cube.h"
#include "Window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "windows.h"
#include <crtdbg.h>


struct MeshObject
{
    Repeater* mesh;
    std::string name;
    bool selected = false;
};

class Scene
{
public:
    Scene(Camera *camera, ScreenSettings* screenSettings);
    ~Scene();

    void renderScene();
    void update();
    bool updateObjectMesh(int idx);

    // Delete and recreate instanced with a different shader if changing instanced rendering state
    void updatePlaneInstanced(bool instanced, int idx);
    void updateCubeInstanced(bool instanced, int idx);

    void addCube();
    void addPlane();

    std::vector<MeshObject*>* getMeshList();

private:
    void createPlane(bool instanced, Plane*& plane);
    void createCube(bool instanced, Cube*& cube);
    void renderPlane();
    int getTriangleCount();
    void dumpMemory();

    Camera* m_camera;
    Shader* m_ourShader;
    Shader* m_ourShaderInstanced;
    Texture* m_smiley_texture;
    ScreenSettings* m_screenSettings;

    unsigned int m_VAO;
    unsigned int m_EBO;

    int m_faceCounter;

    unsigned int m_texture1;
    unsigned int m_texture2;

    int m_columns;
    int m_rows;
    float m_scale;

    bool m_instanced;
    bool m_instanced_cube;

    std::vector<MeshObject*>* m_meshList;
};

#endif // SCENE_H