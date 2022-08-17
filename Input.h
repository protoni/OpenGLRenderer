#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"


class Input
{
public:

    Input(GLFWwindow *window, Camera *camera);
    ~Input();

    void processInput(GLFWwindow* window, float deltaTime);

private:
    GLFWwindow *m_window;
    Camera *m_camera;
    
};

#endif // INPUT_H