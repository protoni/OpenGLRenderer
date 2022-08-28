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
    void update(double deltaTime);
    void cleanup();
    void debugMode();
    void objectLayout(bool* p_open);

private:
    void init();
    void draw();
    void render();
    void updateWireframeMode();
    void updatePlaneSize();
    void updateCubeCount();
    void updateInstancedPlaneMode();
    void updateInstancedCubeMode();


    Window* m_window;
    Scene* m_scene;
    bool m_debugModeOn;
    bool m_wireframeModeOn;
    bool m_instancedPlaneOn;
    bool m_instancedCubeOn;

    RepeaterState* m_planeState;

    float m_debounceCounter;
    
};


#endif // DEBUG_UI_H