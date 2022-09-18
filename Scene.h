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
#include "MaterialBase.h"
#include "Model.h"
#include "MeshListHandler.h"
#include "MeshObject.h"

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


class Scene
{
public:
    Scene(Camera *camera, ScreenSettings* screenSettings);
    ~Scene();

    void renderScene();
    bool updateObjectMesh(int idx);

    void updateMeshShader(bool instanced, int idx);

    void addCube();
    void addPlane();
    void addTriangle();
    void addSphere();
    void addCustom();
    void addLight();
    void addDirectionalLight();
    void addSpotLight();
    void addReflectingCube();
    void addModel();
    void updateMeshPointer(int direction, bool multiselect = false);
    void resetMeshPointer();
    void deleteInstancedMesh(int selected);
    void deleteDirectionalLight(int selected);

    std::vector<MeshObject*>* getMeshList();

    void deleteObject(int idx);
    void deleteModel(int idx);
    void clean();
    int getTriangleCount();
    int getObjectCount();

    int getMeshPointer() { return m_meshPointer; }

    /* Control multi-pick mode ( individual mesh picker )*/
    void toggleMultiPickMode() { m_multiPickMode = !m_multiPickMode; }
    void setMultiPickMode(bool mode) { m_multiPickMode = mode; }

    /* Add currently selected mesh to multi select vector */
    void multiPick();

    void updateMeshMaterial(int selected, const std::string& newMaterial);

private:
    void draw(int idx, glm::mat4& projection, glm::mat4& view);
    void drawModel(int idx, glm::mat4& projection, glm::mat4& view);
    int getSelectedMeshIndex();
    void highlightSelectedMeshes();
    void renderPointLight(int idx, Shader* shader);
    void renderDirectionalLight(int idx, Shader* shader);
    void renderSpotLight(int idx, Shader* shader);

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

    glm::vec3 m_lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

    // Lights
    std::vector<MeshObject*> m_pointLights;
    std::vector<MeshObject*> m_directionalLights;
    std::vector<MeshObject*> m_spotLights;

};

#endif // SCENE_H