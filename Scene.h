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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Scene
{
public:
    Scene(Camera *camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);
    ~Scene();

    void renderScene();
    void update();
    void updatePlane();
    void updateCube();

    // Delete and recreate instanced with a different shader if changing instanced rendering state
    void updatePlaneInstanced(bool state);
    void updateCubeInstanced(bool state);

    RepeaterState* getPlaneState();
    RepeaterState* getCubeState();

private:
    void createPlane();
    void createCube();
    void renderPlane();
    int getTriangleCount();

    Camera *m_camera;
    Shader *m_ourShader;
    Shader * m_ourShaderInstanced;
    Plane* m_plane_mesh;
    Cube* m_cube_mesh;
    Texture* m_smiley_texture;

    unsigned int m_screenWidth;
    unsigned int m_screenHeight;
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

    std::vector<Mesh*> m_meshList;
    
};

#endif // SCENE_H