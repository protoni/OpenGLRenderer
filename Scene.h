#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Scene
{
public:
    Scene(Camera *camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);
    ~Scene();

    void renderScene();
    void update();
    void resizeHexagon(float multiplier);
    float getMultiplier();

private:
    void createHexagon();
    void renderHexagon();

    Camera *m_camera;
    Shader *m_ourShader;
    Mesh* m_hexagon_mesh;
    Texture* m_smiley_texture;

    unsigned int m_screenWidth;
    unsigned int m_screenHeight;
    unsigned int m_VAO;
    unsigned int m_EBO;

    int m_faceCounter;

    unsigned int m_texture1;
    unsigned int m_texture2;

    float m_multiplier;
    
};

#endif // SCENE_H