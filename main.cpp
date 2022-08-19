#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string.h>
#include <chrono>
#include <thread>

#include "Scene.h"
#include "Shader.h"
#include "Camera.h"
#include "Input.h"
#include "Window.h"

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float ratio = 0.2f;

Camera *camera;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Mouse
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

// Time
float deltaTime = 0.0f;   // Time between current frame and last frame
float lastFrame = 0.0f;   // Time of last frame
float secondFrame = 0.0f; // Count seconds
float fpsCounter = 0.0f;  // Count frames per second
float fpsLimit = 120.0f;   // MAX FPS
float frame_time = (1.0f / fpsLimit) * 1000;
float sleep_time = 0.0f;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (camera) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse) // initially set to true
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
        lastX = xpos;
        lastY = ypos;

        camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (camera) {
        camera->ProcessMouseScroll(static_cast<float>(yoffset));
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv)
{
    // Create main window
    Window* window = new Window(SCR_WIDTH, SCR_HEIGHT);
    window->init();

    // Check how many vertex attributes are supported
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // Create camera
    camera = new Camera();
    window->setCamera(camera);

    // Create scene
    Scene *scene = new Scene(camera, SCR_WIDTH, SCR_HEIGHT);

    // Create input handler
    Input *input = new Input(window->get(), camera);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Enable blending
    //glEnable(GL_BLEND);

    // Render loop
    while (!window->shouldExit())
    {
        // Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Limit FPS
        sleep_time = frame_time - ( deltaTime * 1000);
        std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(sleep_time)));

        // Update inputs
        input->processInput(window->get(), deltaTime);

        // Clear background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 120fps -> 130fps

        // Update frame counter
        secondFrame += deltaTime;
        fpsCounter++;

        // Render the scene
        scene->renderScene();

        // Do something every second
        if (secondFrame >= 1.0f) {
            secondFrame = 0.0f;

            // Print out FPS and delta time
            std::cout << "deltaTime: " << deltaTime << std::endl;
            std::cout << "FPS: " << fpsCounter << std::endl;
            fpsCounter = 0.0f;

            // Update the scene
            scene->update();
        }

        window->swapBuffers();
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();

    return 0;
}