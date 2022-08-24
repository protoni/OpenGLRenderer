#ifndef DEBUG_UI_H
#define DEBUG_UI_H

#include "Window.h"
#include "Scene.h"

class DebugUi
{
public:
    DebugUi(Window* window, Scene* scene);
    ~DebugUi();

    void newWindow();
    void update(float deltaTime);
    void cleanup();
    void debugMode();

private:
    void init();
    void draw();
    void render();
    void updateWireframeMode();
    void updatePlaneSize();

    Window* m_window;
    Scene* m_scene;
    bool m_debugModeOn;
    bool m_wireframeModeOn;

    int m_planeSize;
    int m_planeSizeZ;
    float m_debounceCounter;
    float m_planeScale;
};


#endif // DEBUG_UI_H