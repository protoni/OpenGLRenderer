#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

#include "Camera.h"

struct ScreenSettings
{
    unsigned int width = 800;
    unsigned int height = 600;
    const std::string title = "OpenGLRenderer";
};

class Window
{
public:
    Window(ScreenSettings* settings);
    ~Window();

    bool init();
    void setCamera(Camera* camera);
    GLFWwindow* get();
    bool shouldExit();
    void swapBuffers();
    void debugMode();
    void wireframeMode();
    bool getWireframeMode();
    void setWireframeMode(bool state);

    Camera* m_camera;

private:
    // Callback functions
    static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* m_window;
    ScreenSettings* m_windowSettings;

    bool m_debugModeOn;
    bool m_wireframeModeOn;

    // Mouse
    bool m_firstMouse;
    double m_lastX;
    double m_lastY;
};

#endif // WINDOW_H