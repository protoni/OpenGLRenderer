#ifndef SCENE_H
#define SCENE_H

//#include <glad/glad.h>
#include <glad_opengl4_3/glad.h>
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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "windows.h"
#include <crtdbg.h>
#include <thread>

enum MeshInstanceDirections
{
    Left,
    Right,
    Up,
    Down,
    Forward,
    Backward
};

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
    void addSphere();
    void addCustom();
    void updateMeshPointer(int direction, bool multiselect = false);
    void resetMeshPointer();
    void deleteInstancedMesh(int selected);

    std::vector<MeshObject*>* getMeshList();

    void deleteObject(int idx);
    void clean();
    int getTriangleCount();
    int getObjectCount();

    int getMeshPointer() { return m_meshPointer; }

    /* Control multi-pick mode ( individual mesh picker )*/
    void toggleMultiPickMode() { m_multiPickMode = !m_multiPickMode; }
    void setMultiPickMode(bool mode) { m_multiPickMode = mode; }

    /* Add currently selected mesh to multi select vector */
    void multiPick();

private:
    void createPlane(bool instanced, Plane*& plane);
    void createCube(bool instanced, Cube*& cube);
    void renderPlane();
    void dumpMemory();
    void draw(int idx, glm::mat4& projection, glm::mat4& view);
    int getSelectedMeshIndex();
    void highlightSelectedMeshes();

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
    unsigned int m_meshPointer;

    bool m_instanced;
    bool m_instanced_cube;

    std::vector<MeshObject*>* m_meshList;
    std::vector<int> m_multiSelectVec;

    int m_oldMultiSelectVecSize = 0;
    unsigned int m_oldMeshPointer = 0;

    GLuint m_ssbo;

    bool m_multiPickMode = false;
};

#endif // SCENE_H