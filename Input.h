#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "DebugUi.h"
#include "Scene.h"


class Input
{
public:

    Input(Window *window, Camera *camera, DebugUi* debugUi);
    ~Input();

    void processInput(double deltaTime);

private:
    Window* m_window;
    Camera* m_camera;
    DebugUi* m_debugUi;

    double m_debounceCounter;
    
};

#endif // INPUT_H