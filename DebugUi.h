#ifndef DEBUG_UI_H
#define DEBUG_UI_H

#include "Window.h"

class DebugUi
{
public:
    DebugUi(Window* window);
    ~DebugUi();

    void newWindow();
    void draw();
    void render();
    void cleanup();
    void debugMode();

private:
    void init();

    Window* m_window;
    bool m_debugModeOn;
};


#endif // DEBUG_UI_H