#include "Input.h"
#include <iostream>

#include "DebugMacros.h"

Input::Input(Window* window, Camera* camera, DebugUi* debugUi, Scene* scene) :
    m_window(window), m_camera(camera), m_debugUi(debugUi),
    m_debounceCounter(0), m_scene(scene)
{

}

Input::~Input()
{
}

void Input::processInput(double deltaTime)
{
    m_debounceCounter += deltaTime;

    // Handle exit input
    if (glfwGetKey(m_window->get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->get(), true);

    // Handle camera movement input
    const float cameraSpeed = 2.5f * (float)deltaTime;
    if (glfwGetKey(m_window->get(), GLFW_KEY_W) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(m_window->get(), GLFW_KEY_S) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(m_window->get(), GLFW_KEY_A) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(m_window->get(), GLFW_KEY_D) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
    if (glfwGetKey(m_window->get(), GLFW_KEY_SPACE) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::UP, deltaTime);
    if (glfwGetKey(m_window->get(), GLFW_KEY_X) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::DOWN, deltaTime);

    bool changed = false;
    
    // Limit actions to 50ms
    if (m_debounceCounter >= .05) {
        int direction = -1;
        bool multiselect = false;

        // Move mesh pointer
        if (glfwGetKey(m_window->get(), GLFW_KEY_LEFT) == GLFW_PRESS) {
            //m_scene->updateMeshPointer(MeshInstanceDirections::Left);
            direction = MeshInstanceDirections::Left;
            changed = true;
        }

        if (glfwGetKey(m_window->get(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
            //m_scene->updateMeshPointer(MeshInstanceDirections::Right);
            direction = MeshInstanceDirections::Right;
            changed = true;
        }

        if (glfwGetKey(m_window->get(), GLFW_KEY_UP) == GLFW_PRESS) {
            //m_scene->updateMeshPointer(MeshInstanceDirections::Forward);
            direction = MeshInstanceDirections::Forward;
            changed = true;
        }

        if (glfwGetKey(m_window->get(), GLFW_KEY_DOWN) == GLFW_PRESS) {
            //m_scene->updateMeshPointer(MeshInstanceDirections::Backward);
            direction = MeshInstanceDirections::Backward;
            changed = true;
        }

        if (glfwGetKey(m_window->get(), GLFW_KEY_COMMA) == GLFW_PRESS) {
            //m_scene->updateMeshPointer(MeshInstanceDirections::Up);
            direction = MeshInstanceDirections::Up;
            changed = true;
        }

        if (glfwGetKey(m_window->get(), GLFW_KEY_PERIOD) == GLFW_PRESS) {
            //m_scene->updateMeshPointer(MeshInstanceDirections::Down);
            direction = MeshInstanceDirections::Down;
            changed = true;
        }

        if (glfwGetKey(m_window->get(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            multiselect = true;
        }

        if (glfwGetKey(m_window->get(), GLFW_KEY_DELETE) == GLFW_PRESS) {
            m_scene->deleteInstancedMesh(m_debugUi->getSelectedInstance());
            changed = true;
        }

        if (direction >= 0) {
            if(multiselect)
                m_scene->updateMeshPointer(direction, true);
            else
                m_scene->updateMeshPointer(direction);
        }
    }

    // Limit actions to 500ms
    if (m_debounceCounter >= .5) {

        // Toggle debug UI
        if (glfwGetKey(m_window->get(), GLFW_KEY_TAB) == GLFW_PRESS) {
        
            m_window->debugMode();
            m_debugUi->debugMode();
            std::cout << "dbg mode!" << std::endl;
            changed = true;
        }
    }

    if (changed)
        m_debounceCounter = 0;
}


