#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"

class Window
{
public:
    Window(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);
    ~Window();

    bool init();
    void setCamera(Camera* camera);
    GLFWwindow* get();
    bool shouldExit();
    void swapBuffers();

    Camera* m_camera;

private:
    // Callback functions
    static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* m_window;
    

    unsigned int m_windowWidth;
    unsigned int m_windowHeight;

    // Mouse
    bool m_firstMouse;
    float m_lastX;
    float m_lastY;
};

#endif // WINDOW_H