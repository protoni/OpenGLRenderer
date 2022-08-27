#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Plane.h"

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
    void updatePlane(int rows, int columns, float scale);
    void changePlaneInstanced(bool instanced);
    bool getPlaneInstanceMode();

private:
    void createPlane();
    void renderPlane();

    Camera *m_camera;
    Shader *m_ourShader;
    Shader * m_ourShaderInstanced;
    Plane* m_plane_mesh;
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

    std::vector<Mesh*> m_meshList;
    
};

#endif // SCENE_H