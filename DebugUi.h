#ifndef DEBUG_UI_H
#define DEBUG_UI_H

#include "Window.h"

class DebugUi
{
public:
    DebugUi(Window* window);
    ~DebugUi();

    void newWindow();
    void update();
    void cleanup();
    void debugMode();

private:
    void init();
    void draw();
    void render();
    void updateWireframeMode();

    Window* m_window;
    bool m_debugModeOn;
    bool m_wireframeModeOn;
};


#endif // DEBUG_UI_H