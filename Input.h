#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "DebugUi.h"


class Input
{
public:

    Input(Window *window, Camera *camera, DebugUi* debugUi);
    ~Input();

    void processInput(float deltaTime);

private:
    Window* m_window;
    Camera* m_camera;
    DebugUi* m_debugUi;

    float m_debounceCounter;
    
};

#endif // INPUT_H