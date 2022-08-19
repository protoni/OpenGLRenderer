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
    std::cout << "Init OpenGLRenderer" << std::endl;

    glfwInit();

    // Disable window borders
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    
    // Make window background transparent
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGLRenderer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    // Set window viewport dimensions
    glViewport(0, 0, 800, 600);

    // Set window resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Check how many vertex attributes are supported
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // Create camera
    camera = new Camera();

    // Create scene
    Scene *scene = new Scene(camera, SCR_WIDTH, SCR_HEIGHT);

    // Create input handler
    Input *input = new Input(window, camera);

    // Camera rotation radius
    const float radius = 10.0f;

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Disable cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set mouse input callback
    glfwSetCursorPosCallback(window, mouse_callback);

    // Set mouse scroll callback
    glfwSetScrollCallback(window, scroll_callback);

    // Enable blending
    //glEnable(GL_BLEND);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Limit FPS
        sleep_time = frame_time - ( deltaTime * 1000);
        std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(sleep_time)));

        // Update inputs
        input->processInput(window, deltaTime);

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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();

    return 0;
}