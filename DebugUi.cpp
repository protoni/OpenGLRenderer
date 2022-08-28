#include "DebugUi.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

DebugUi::DebugUi(Window* window, Scene* scene) : m_window(window), m_scene(scene),
    m_debugModeOn(false), m_wireframeModeOn(false), m_debounceCounter(0.0),
    m_planeState(NULL), m_cubeState(NULL)
{
    init();

    // Init local plane state
    m_planeState = new RepeaterState();

    // Init local cube state
    m_cubeState = new RepeaterState();
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
        ImGui::Checkbox("Instanced Plane", &m_planeState->instanced);
        ImGui::SliderInt("Plane Columns", &m_planeState->columnCount, 1, 500);
        ImGui::SliderInt("Plane Rows", &m_planeState->rowCount, 1, 500);
        ImGui::SliderInt("Plane Stacks", &m_planeState->stackCount, 1, 500);
        ImGui::SliderFloat("Plane scale", &m_planeState->scale, 0.01f, 10.0f);
        ImGui::SliderFloat("Plane padding", &m_planeState->padding, 0.0f, 10.0f);
        ImGui::Text("");
        ImGui::Text("Cube");
        ImGui::Checkbox("Instanced Cube", &m_cubeState->instanced);
        ImGui::SliderInt("Cube Columns", &m_cubeState->columnCount, 1, 500);
        ImGui::SliderInt("Cube Rows", &m_cubeState->rowCount, 1, 500);
        ImGui::SliderInt("Cube stacks", &m_cubeState->stackCount, 1, 500);
        ImGui::SliderFloat("Cube scale", &m_cubeState->scale, 0.01f, 10.0f);
        ImGui::SliderFloat("Cube padding", &m_cubeState->padding, 0.0f, 10.0f);
        ImGui::Text("");
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

void DebugUi::updateInstancedPlaneMode()
{
    if (m_debugModeOn) {
        if (m_debounceCounter >= 0.1f) {
            RepeaterState* planeState = m_scene->getPlaneState();

            if (m_planeState->instanced != planeState->instanced) {
                planeState->instanced = m_planeState->instanced;
                
                m_scene->updatePlaneInstanced(m_planeState->instanced);

                m_debounceCounter = 0;
            }
        }
    }
}

void DebugUi::updateInstancedCubeMode()
{
    if (m_debugModeOn) {
        if (m_debounceCounter >= 0.1f) {

            RepeaterState* cubeState = m_scene->getCubeState();

            if (m_cubeState->instanced != cubeState->instanced) {
                cubeState->instanced = m_cubeState->instanced;
                
                m_scene->updateCubeInstanced(m_cubeState->instanced);
                m_debounceCounter = 0;
            }
        }
    }
}

void DebugUi::updatePlaneSize()
{
    if (m_debugModeOn) {
        if (m_debounceCounter >= 0.1f) {
            RepeaterState* planeState = m_scene->getPlaneState();

            if (m_planeState->columnCount != planeState->columnCount ||
                m_planeState->rowCount != planeState->rowCount ||
                m_planeState->stackCount != planeState->stackCount ||
                m_planeState->scale != planeState->scale ||
                m_planeState->padding != planeState->padding) {

                    planeState->columnCount = m_planeState->columnCount;
                    planeState->rowCount = m_planeState->rowCount;
                    planeState->stackCount = m_planeState->stackCount;
                    planeState->scale = m_planeState->scale;
                    planeState->padding = m_planeState->padding;

                    m_scene->updatePlane();
                    m_debounceCounter = 0;
            }
        }
    }
}

void DebugUi::updateCubeCount()
{
    if (m_debugModeOn) {
        if (m_debounceCounter >= 0.1f) {
            
            RepeaterState* cubeState = m_scene->getCubeState();

            if (m_cubeState->columnCount != cubeState->columnCount ||
                m_cubeState->rowCount != cubeState->rowCount ||
                m_cubeState->stackCount != cubeState->stackCount ||
                m_cubeState->scale != cubeState->scale ||
                m_cubeState->padding != cubeState->padding) {

                    cubeState->columnCount = m_cubeState->columnCount;
                    cubeState->rowCount = m_cubeState->rowCount;
                    cubeState->stackCount = m_cubeState->stackCount;
                    cubeState->scale = m_cubeState->scale;
                    cubeState->padding = m_cubeState->padding;

                    m_scene->updateCube();
                    m_debounceCounter = 0;
            }
        }
    }
}

void DebugUi::update(double deltaTime)
{
    m_debounceCounter += (float)deltaTime;
    draw();
    render();
    updateWireframeMode();
    updatePlaneSize();
    updateCubeCount();
    updateInstancedPlaneMode();
    updateInstancedCubeMode();
    
}