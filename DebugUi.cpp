#include "DebugUi.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>

DebugUi::DebugUi(Window* window, Scene* scene) : m_window(window), m_scene(scene),
    m_debugModeOn(false), m_wireframeModeOn(false), m_debounceCounter(0.0),
    m_planeState(NULL)
{
    init();

    // Init local plane state
    m_planeState = new RepeaterState();
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

void DebugUi::objectLayout(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Objects", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::Button("Add Cube", ImVec2(100, 0)))
            m_scene->addCube();

        ImGui::SameLine();

        if (ImGui::Button("Add Plane", ImVec2(100, 0)))
            m_scene->addPlane();

        // Left
        static int selected = 0;
        {
            std::vector<MeshObject*> meshList = *m_scene->getMeshList();
            ImGui::BeginChild("left pane", ImVec2(150, 0), true);
            for (int i = 0; i < meshList.size(); i++)
            {
                if (ImGui::Selectable(meshList.at(i)->name.c_str(), selected == i))
                    selected = i;
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();

        // Right
        {
            std::vector<MeshObject*>* meshList = m_scene->getMeshList();

            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us

            if (meshList->size() > selected) {
                ImGui::Text(meshList->at(selected)->name.c_str(), selected);

                if (ImGui::Button("Remove Object", ImVec2(100, 0))) {
                    meshList->erase(meshList->begin() + selected);

                    ImGui::EndChild();
                    ImGui::EndGroup();
                    ImGui::End();
                    return;
                }
            
                ImGui::Separator();
                
                // Load current values
                RepeaterState* state = meshList->at(selected)->mesh->getState();
                m_planeState->instanced = state->instanced;
                m_planeState->columnCount = state->columnCount;
                m_planeState->rowCount = state->rowCount;
                m_planeState->stackCount = state->stackCount;
                m_planeState->scale = state->scale;
                m_planeState->padding = state->padding;
                
                // Set current values and draw settings
                ImGui::Checkbox("Instanced", &m_planeState->instanced);
                ImGui::SliderInt("Columns", &m_planeState->columnCount, 1, 500);
                ImGui::SliderInt("Rows", &m_planeState->rowCount, 1, 500);
                ImGui::SliderInt("Stacks", &m_planeState->stackCount, 1, 500);
                ImGui::SliderFloat("Scale", &m_planeState->scale, 0.01f, 10.0f);
                ImGui::SliderFloat("Padding", &m_planeState->padding, 0.0f, 10.0f);
                ImGui::Separator();
                
                // Check if settings changed
                if (m_planeState->instanced != state->instanced) {
                    state->instanced = m_planeState->instanced;

                    if (meshList->at(selected)->name.find("Cube") != std::string::npos)
                        m_scene->updateCubeInstanced(m_planeState->instanced, selected);
                    else if (meshList->at(selected)->name.find("Plane") != std::string::npos)
                        m_scene->updatePlaneInstanced(m_planeState->instanced, selected);

                    m_scene->updateObjectMesh(selected);
                }

                if (m_planeState->columnCount != state->columnCount ||
                    m_planeState->rowCount != state->rowCount ||
                    m_planeState->stackCount != state->stackCount ||
                    m_planeState->scale != state->scale ||
                    m_planeState->padding != state->padding
                ) {
                    state->columnCount = m_planeState->columnCount;
                    state->rowCount = m_planeState->rowCount;
                    state->stackCount = m_planeState->stackCount;
                    state->scale = m_planeState->scale;
                    state->padding = m_planeState->padding;

                    m_scene->updateObjectMesh(selected);
                }
            }

            ImGui::EndChild();
            ImGui::EndGroup();
        }
    }
    ImGui::End();
}

void DebugUi::draw()
{
    if (m_debugModeOn) {
        ImGui::Begin("Settings");
        ImGui::Text("");
        ImGui::Checkbox("Wireframe Mode", &m_wireframeModeOn);
        ImGui::Text("");

        bool open;
        objectLayout(&open);

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

void DebugUi::update(double deltaTime)
{
    m_debounceCounter += (float)deltaTime;
    draw();
    render();
    updateWireframeMode();
}