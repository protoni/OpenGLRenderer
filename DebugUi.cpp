#include "DebugUi.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

DebugUi::DebugUi(Window* window) : m_window(window), m_debugModeOn(false)
{
    init();
}

DebugUi::~DebugUi()
{
}

void DebugUi::init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window->get(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void DebugUi::newWindow()
{
    if (m_debugModeOn) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
}

void DebugUi::draw()
{
    if (m_debugModeOn) {
        ImGui::Begin("Test title");
        ImGui::Text("Test text");
        ImGui::End();
    }
}

void DebugUi::render()
{
    if (m_debugModeOn) {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

void DebugUi::cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DebugUi::debugMode()
{
    m_debugModeOn = !m_debugModeOn;
}