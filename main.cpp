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


#include "windows.h"
#include <crtdbg.h>

#include "DebugMacros.h"

#define _CRTDBG_MAP_ALLOC //to get more details

//#ifdef _DEBUG
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
//// allocations to be of _CLIENT_BLOCK type
//#else
//#define DBG_NEW new
//#endif

// Memory leak detection
_CrtMemState sOld;
_CrtMemState sNew;
_CrtMemState sDiff;


// Time
double deltaTime = 0.0f;   // Time between current frame and last frame
double lastFrame = 0.0f;   // Time of last frame
double secondFrame = 0.0f; // Count seconds
double fpsCounter = 0.0f;  // Count frames per second
double fpsLimit = 120.0f;   // MAX FPS
double frame_time = (1.0f / fpsLimit) * 1000;
double sleep_time = 0.0f;
double fps = 0.0f;


int main(int argc, char** argv)
{
    // Auto dump memory leak info
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Take a snapshot of heap memory
    _CrtMemCheckpoint(&sOld); 

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
    Input* input = new Input(window, camera, debugUi, scene);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable blending ( transparency )
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render loop
    while (!window->shouldExit())
    {
        // Calculate delta time
        double currentFrame = glfwGetTime();
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
            fps = fpsCounter;
            fpsCounter = 0.0f;

            // Update the scene
            scene->update();
        }

        // Draw debug UI
        debugUi->update(deltaTime, fps);

        window->swapBuffers();
        glfwPollEvents();
    }

    // Cleanup
    scene->clean();
    debugUi->cleanup();
    glfwTerminate();

    delete window;
    delete camera;
    delete scene;
    delete debugUi;
    delete input;

    

    std::cout << "Exiting.." << std::endl;

    _CrtMemCheckpoint(&sNew); //take a snapshot 
    if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
    {
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

        OutputDebugString(L"-----------_CrtMemDumpStatistics ---------");
        _CrtMemDumpStatistics(&sDiff);
        //OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------");
        //_CrtMemDumpAllObjectsSince(&sOld);
        OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------");
        _CrtDumpMemoryLeaks();

        
    }

    return 0;
}