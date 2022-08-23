#include "Input.h"
#include <iostream>

Input::Input(Window* window, Camera* camera, DebugUi* debugUi) : 
    m_window(window), m_camera(camera), m_debugUi(debugUi),
    m_debounceCounter(0)
{

}

Input::~Input()
{
}

void Input::processInput(float deltaTime)
{
    m_debounceCounter += deltaTime;

    // Handle exit input
    if (glfwGetKey(m_window->get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->get(), true);

    // Handle camera movement input
    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(m_window->get(), GLFW_KEY_W) == GLFW_PRESS)
        m_camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(m_window->get(), GLFW_KEY_S) == GLFW_PRESS)
        m_camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(m_window->get(), GLFW_KEY_A) == GLFW_PRESS)
        m_camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(m_window->get(), GLFW_KEY_D) == GLFW_PRESS)
        m_camera->ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(m_window->get(), GLFW_KEY_SPACE) == GLFW_PRESS)
        m_camera->ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(m_window->get(), GLFW_KEY_X) == GLFW_PRESS)
        m_camera->ProcessKeyboard(DOWN, deltaTime);

    // Toggle debug UI
    if (m_debounceCounter >= .5) {
        if (glfwGetKey(m_window->get(), GLFW_KEY_TAB) == GLFW_PRESS) {
        
            m_window->debugMode();
            m_debugUi->debugMode();
            std::cout << "dbg mode!" << std::endl;
            m_debounceCounter = 0;
        }

        
    }
}


