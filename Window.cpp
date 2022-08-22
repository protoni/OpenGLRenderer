#include "Window.h"

#include <iostream>



Window::Window(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT) : 
    m_window(NULL), m_camera(NULL), m_windowWidth(WINDOW_WIDTH), m_windowHeight(WINDOW_HEIGHT),
    m_firstMouse(true), m_lastX(m_windowWidth / 2.0), m_lastY(m_windowHeight / 2.0),
    m_debugModeOn(false)
{

}

Window::~Window()
{

}

bool Window::init()
{
    std::cout << "Init OpenGLRenderer" << std::endl;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a window
    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "OpenGLRenderer", NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_window);

    // Init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    // Set window viewport dimensions
    glViewport(0, 0, m_windowWidth, m_windowHeight);

    // Set window resize callback
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    // Disable cursor
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set mouse input callback
    glfwSetCursorPosCallback(m_window, mouseCallback);

    // Set mouse scroll callback
    glfwSetScrollCallback(m_window, scrollCallback);

    // Set pointer to this object
    glfwSetWindowUserPointer(m_window, this);

    return true;
}

void Window::setCamera(Camera* camera)
{
    m_camera = camera;
}

void Window::mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
    Window* windowObject = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (windowObject->m_debugModeOn) {
        windowObject->m_firstMouse = true;
        return;
    }

    if (windowObject->m_camera) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (windowObject->m_firstMouse) // initially set to true
        {
            windowObject->m_lastX = xpos;
            windowObject->m_lastY = ypos;
            windowObject->m_firstMouse = false;
        }

        float xoffset = xpos - windowObject->m_lastX;
        float yoffset = windowObject->m_lastY - ypos; // reversed since y-coordinates range from bottom to top
        windowObject->m_lastX = xpos;
        windowObject->m_lastY = ypos;

        windowObject->m_camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

bool Window::shouldExit()
{
    return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(m_window);
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->m_camera) {
        theWindow->m_camera->ProcessMouseScroll(static_cast<float>(yoffset));
    }
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow* Window::get()
{
    return m_window;
}

void Window::debugMode()
{
    m_debugModeOn = !m_debugModeOn;

    if (m_debugModeOn) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}