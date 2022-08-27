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
    void updateInstancedPlaneMode();

    Window* m_window;
    Scene* m_scene;
    bool m_debugModeOn;
    bool m_wireframeModeOn;
    bool m_instancedPlaneOn;

    int m_planeSize;
    int m_planeSizeZ;
    float m_planeScale;

    int m_planeSize_old;
    int m_planeSizeZ_old;
    float m_planeScale_old;

    float m_debounceCounter;
    
};


#endif // DEBUG_UI_H