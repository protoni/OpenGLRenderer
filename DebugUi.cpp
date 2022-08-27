#include "DebugUi.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

DebugUi::DebugUi(Window* window, Scene* scene) : m_window(window), m_scene(scene),
    m_debugModeOn(false), m_wireframeModeOn(false), m_planeSize(1), m_debounceCounter(0.0),
    m_planeScale(1.0f), m_planeSizeZ(1), m_planeSize_old(1), m_planeScale_old(1.0f), m_planeSizeZ_old(1)
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
        ImGui::Begin("Settings");
        ImGui::Text("");
        ImGui::Checkbox("Wireframe Mode", &m_wireframeModeOn);
        ImGui::Text("");
        ImGui::Text("Plane");
        ImGui::SliderInt("Plane Columns", &m_planeSize, 1, 500);
        ImGui::SliderInt("Plane Rows", &m_planeSizeZ, 1, 500);
        ImGui::SliderFloat("Plane scale", &m_planeScale, 0.01f, 10.0f);
        ImGui::Text("");
        ImGui::Text("Cube");
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

void DebugUi::updateWireframeMode()
{
    if (m_wireframeModeOn != m_window->getWireframeMode()) {
        m_window->setWireframeMode(m_wireframeModeOn);
    }
}

void DebugUi::updatePlaneSize()
{
    if (m_debugModeOn) {
        if (m_debounceCounter >= 0.1f) {
            m_debounceCounter = 0;

            if (m_planeSize != m_planeSize_old ||
                m_planeSizeZ != m_planeSizeZ_old ||
                m_planeScale != m_planeScale_old) {
                m_scene->updatePlane(m_planeSize, m_planeSizeZ, m_planeScale);

                m_planeSize_old = m_planeSize;
                m_planeSizeZ_old = m_planeSizeZ;
                m_planeScale_old = m_planeScale;
            }
        }
    }
}

void DebugUi::update(float deltaTime)
{
    m_debounceCounter += deltaTime;
    draw();
    render();
    updateWireframeMode();
    updatePlaneSize();
}