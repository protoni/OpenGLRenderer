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
    void update(double deltaTime, double fps);
    void cleanup();
    void debugMode();
    void objectLayout(bool* p_open);
    int getSelectedInstance() { return m_selected; }

private:
    void init();
    void draw();
    void render();
    void updateWireframeMode();
    void updatePlaneSize();
    void updateCubeCount();
    void updateInstancedPlaneMode();
    void updateInstancedCubeMode();
    void updateInfoWindow();
    void showInfoWindow(bool* p_open);
    void objectSettings(int selected);
    void meshSettings();

    Window* m_window;
    Scene* m_scene;
    bool m_debugModeOn;
    bool m_wireframeModeOn;
    bool m_infoWindowOn;
    bool m_instancedPlaneOn;
    bool m_instancedCubeOn;

    RepeaterState* m_planeState;

    float m_debounceCounter;
    
    float m_fps;
    double m_deltaTime;

    int m_selected;
};


#endif // DEBUG_UI_H