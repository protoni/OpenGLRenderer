#include "DebugUi.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>

#include "DebugMacros.h"

DebugUi::DebugUi(Window* window, Scene* scene) : m_window(window), m_scene(scene),
    m_debugModeOn(false), m_wireframeModeOn(false), m_debounceCounter(0.0),
    m_planeState(NULL), m_infoWindowOn(false), m_fps(0.0), m_deltaTime(0.0)
{
    init();

    // Init local plane state
    m_planeState = new RepeaterState();
}

DebugUi::~DebugUi()
{
    if (m_planeState) {
        delete m_planeState;
        m_planeState = NULL;
    }
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
    if (m_debugModeOn || m_infoWindowOn) {
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

        ImGui::SameLine();

        if (ImGui::Button("Add Triangle", ImVec2(100, 0)))
            m_scene->addTriangle();

        if (ImGui::Button("Add Sphere", ImVec2(100, 0)))
            m_scene->addSphere();

        ImGui::SameLine();

        if (ImGui::Button("Add Custom", ImVec2(100, 0)))
            m_scene->addCustom();

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

            // Reset mesh pointer
            if (selected < meshList->size()) {
                if (!meshList->at(selected)->selected)
                    m_scene->resetMeshPointer();
            }

            // Unselect all first
            for (int i = 0; i < meshList->size(); i++) {
                meshList->at(i)->selected = false;
            }

            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us

            if (meshList->size() > selected) {
                ImGui::Text(meshList->at(selected)->name.c_str(), selected);

                if (ImGui::Button("Remove Object", ImVec2(100, 0))) {
                    m_scene->deleteObject(selected);

                    ImGui::EndChild();
                    ImGui::EndGroup();
                    ImGui::End();
                    return;
                }
            
                ImGui::Separator();
                
                // Set selected
                meshList->at(selected)->selected = true;

                // Load current values
                RepeaterState* state = meshList->at(selected)->mesh->getState();
                m_planeState->instanced = state->instanced;
                m_planeState->columnCount = state->columnCount;
                m_planeState->rowCount = state->rowCount;
                m_planeState->stackCount = state->stackCount;
                m_planeState->scaleX = state->scaleX;
                m_planeState->scaleY = state->scaleY;
                m_planeState->scaleZ = state->scaleZ;
                m_planeState->paddingX = state->paddingX;
                m_planeState->paddingY = state->paddingY;
                m_planeState->paddingZ = state->paddingZ;
                m_planeState->xOffset = state->xOffset;
                m_planeState->yOffset = state->yOffset;
                m_planeState->zOffset = state->zOffset;

                m_planeState->angle = state->angle;
                m_planeState->xRotation = state->xRotation;
                m_planeState->yRotation = state->yRotation;
                m_planeState->zRotation = state->zRotation;
                
                // Set current values and draw settings
                ImGui::Checkbox("Instanced", &m_planeState->instanced);
                if (ImGui::CollapsingHeader("Repeater")) {
                    ImGui::SliderInt("Columns", &m_planeState->columnCount, 1, 100);
                    ImGui::SliderInt("Rows", &m_planeState->rowCount, 1, 100);
                    ImGui::SliderInt("Stacks", &m_planeState->stackCount, 1, 100);
                }

                //ImGui::Separator();
                if (ImGui::CollapsingHeader("Scale")) {
                    ImGui::SliderFloat("Scale X", &m_planeState->scaleX, 0.01f, 10.0f);
                    ImGui::SliderFloat("Scale Y", &m_planeState->scaleY, 0.01f, 10.0f);
                    ImGui::SliderFloat("Scale Z", &m_planeState->scaleZ, 0.01f, 10.0f);
                }

                //ImGui::Separator();
                if (ImGui::CollapsingHeader("Padding")) {
                    ImGui::SliderFloat("Padding X", &m_planeState->paddingX, 0.0f, 10.0f);
                    ImGui::SliderFloat("Padding Y", &m_planeState->paddingY, 0.0f, 10.0f);
                    ImGui::SliderFloat("Padding Z", &m_planeState->paddingZ, 0.0f, 10.0f);
                }

                //ImGui::Separator();
                if (ImGui::CollapsingHeader("Offset")) {
                    ImGui::SliderFloat("X offset", &m_planeState->xOffset, -5.0f, 5.0f);
                    ImGui::SliderFloat("Y offset", &m_planeState->yOffset, -5.0f, 5.0f);
                    ImGui::SliderFloat("Z offset", &m_planeState->zOffset, -5.0f, 5.0f);
                }
                
                //ImGui::Separator();
                if (ImGui::CollapsingHeader("Rotation")) {
                    ImGui::SliderFloat("Rotation angle", &m_planeState->angle, 0.0f, 360.0f);
                    ImGui::SliderFloat("X Rotation", &m_planeState->xRotation, 0.001f, 1.0f);
                    ImGui::SliderFloat("Y Rotation", &m_planeState->yRotation, 0.001f, 1.0f);
                    ImGui::SliderFloat("Z Rotation", &m_planeState->zRotation, 0.001f, 1.0f);
                }

                // Check if settings changed
                if (m_debounceCounter >= .1f) {
                    if (m_planeState->instanced != state->instanced) {
                        state->instanced = m_planeState->instanced;
                        m_scene->updateMeshShader(m_planeState->instanced, selected);
                        m_debounceCounter = 0;
                    }

                    if (m_planeState->columnCount != state->columnCount ||
                        m_planeState->rowCount != state->rowCount ||
                        m_planeState->stackCount != state->stackCount ||
                        m_planeState->scaleX != state->scaleX ||
                        m_planeState->scaleY != state->scaleY ||
                        m_planeState->scaleZ != state->scaleZ ||
                        m_planeState->paddingX != state->paddingX ||
                        m_planeState->paddingY != state->paddingY ||
                        m_planeState->paddingZ != state->paddingZ ||
                        m_planeState->xOffset != state->xOffset ||
                        m_planeState->yOffset != state->yOffset ||
                        m_planeState->zOffset != state->zOffset ||
                        m_planeState->angle != state->angle ||
                        m_planeState->xRotation != state->xRotation ||
                        m_planeState->yRotation != state->yRotation ||
                        m_planeState->zRotation != state->zRotation
                        ) {
                        state->columnCount = m_planeState->columnCount;
                        state->rowCount = m_planeState->rowCount;
                        state->stackCount = m_planeState->stackCount;
                        state->scaleX = m_planeState->scaleX;
                        state->scaleY = m_planeState->scaleY;
                        state->scaleZ = m_planeState->scaleZ;
                        state->paddingX = m_planeState->paddingX;
                        state->paddingY = m_planeState->paddingY;
                        state->paddingZ = m_planeState->paddingZ;
                        state->xOffset = m_planeState->xOffset;
                        state->yOffset = m_planeState->yOffset;
                        state->zOffset = m_planeState->zOffset;
                        state->angle = m_planeState->angle;
                        state->xRotation = m_planeState->xRotation;
                        state->yRotation = m_planeState->yRotation;
                        state->zRotation = m_planeState->zRotation;

                        std::cout << "Changed!" << std::endl;
                        m_scene->updateObjectMesh(selected);

                        m_debounceCounter = 0;
                    }
                }
            }

            ImGui::EndChild();
            ImGui::EndGroup();
        }
    }
    ImGui::End();
}

void DebugUi::showInfoWindow(bool* p_open)
{
    static int corner = 0;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Info window", p_open, window_flags))
    {
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");

        ImGui::Text("FPS:             %.1f", m_fps);
        ImGui::Text("Delta time:      %.3f ms", m_deltaTime);
        ImGui::Text("Indices:         %d", m_scene->getTriangleCount());
        ImGui::Text("Objects:         %d", m_scene->getObjectCount());
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
        ImGui::Checkbox("Info Window", &m_infoWindowOn);

        bool open;
        objectLayout(&open);

        ImGui::End();
    }   
    if (m_infoWindowOn) {
        bool open;
        showInfoWindow(&open);
    }
}

void DebugUi::render()
{
    if (m_debugModeOn || m_infoWindowOn) {
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

void DebugUi::update(double deltaTime, double fps)
{
    m_debounceCounter += (float)deltaTime;
    m_fps = (float)fps;
    m_deltaTime = deltaTime;
    draw();
    render();
    updateWireframeMode();
}