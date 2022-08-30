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
#include "Repeater.h"
#include "Triangle.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "windows.h"
#include <crtdbg.h>
#include <thread>

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

    void updateMeshShader(bool instanced, int idx);

    void addCube();
    void addPlane();
    void addTriangle();

    std::vector<MeshObject*>* getMeshList();

    void deleteObject(int idx);
    void clean();
    int getTriangleCount();
    int getObjectCount();

private:
    void createPlane(bool instanced, Plane*& plane);
    void createCube(bool instanced, Cube*& cube);
    void renderPlane();
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