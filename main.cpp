#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
#include "DebugUi.h"

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Time
float deltaTime = 0.0f;   // Time between current frame and last frame
float lastFrame = 0.0f;   // Time of last frame
float secondFrame = 0.0f; // Count seconds
float fpsCounter = 0.0f;  // Count frames per second
float fpsLimit = 120.0f;   // MAX FPS
float frame_time = (1.0f / fpsLimit) * 1000;
float sleep_time = 0.0f;


int main(int argc, char** argv)
{
    // Create main window
    ScreenSettings screenSettings;
    Window* window = new Window(&screenSettings);
    window->init();

    // Check how many vertex attributes are supported
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // Create camera
    Camera* camera = new Camera();
    window->setCamera(camera);

    // Create scene
    Scene* scene = new Scene(camera, &screenSettings);

    // Create debug window
    DebugUi* debugUi = new DebugUi(window, scene);

    // Create input handler
    Input* input = new Input(window, camera, debugUi);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable blending ( transparency )
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
        input->processInput(deltaTime);

        // Clear background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 120fps -> 130fps

        // Draw new ImGui debug window
        debugUi->newWindow();

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

        // Draw debug UI
        debugUi->update(deltaTime);

        window->swapBuffers();
        glfwPollEvents();
    }

    // Cleanup
    debugUi->cleanup();
    glfwTerminate();

    return 0;
}