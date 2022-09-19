#include "DebugUi.h"
#include "MaterialBase.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>

#include "DebugMacros.h"
#include "MeshListHandler.h"

DebugUi::DebugUi(Window* window, Scene* scene) : m_window(window), m_scene(scene),
    m_debugModeOn(false), m_wireframeModeOn(false), m_debounceCounter(0.0),
    m_planeState(NULL), m_infoWindowOn(false), m_fps(0.0), m_deltaTime(0.0), m_selected(-1), m_modifiedMesh(-1), m_loadSelectedMesh(true)//,
    //m_meshState(NULL)
{
    init();

    // Init local object instance state so that we can compare changed values when modifying object count
    m_planeState = new RepeaterState();
    m_planeState->transformations = new MeshTransformations();

    // Init local mesh modifying state so that we can compare changed values when modifying meshes
    //m_meshState = new MeshTransformations();
}

DebugUi::~DebugUi()
{
    if (m_planeState) {
        // Free transformations struct pointer
        delete m_planeState->transformations;

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

void DebugUi::materialSettings(int selected)
{
    ImGui::BeginChild("Material settings", ImVec2(0, -(ImGui::GetFrameHeightWithSpacing() + 80)));
    ImGui::Text("Material settings");

    static float ambient[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
    static float diffuse[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
    static float specular[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
    //glm::vec4 ambientVec;
    //glm::vec4 diffuseVec;
    //glm::vec4 specularVec;
    float shininess;

    std::vector<MeshObject*>* meshList = m_scene->getMeshList();
    std::string currentMaterial = meshList->at(selected)->material->name.c_str();


    static int item_current = 0;
    const char* items[] = { "Custom", "Default", "Emerald", "Silver", "GreenRubber", "YellowRubber" };
    //ImGui::Combo("Predefined Material", &item_current, items, IM_ARRAYSIZE(items));

    if (ImGui::BeginCombo("Predefined Material", currentMaterial.c_str()))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const bool is_selected = (item_current == n);
            if (ImGui::Selectable(items[n], is_selected)) {
                item_current = n;
                if (currentMaterial.compare(std::string(items[item_current])) != 0) {
                    std::cout << "Selected: " << items[item_current] << std::endl;
                    m_scene->updateMeshMaterial(selected, std::string(items[item_current]));
                }
                
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

    ImGui::SliderFloat3("Ambient", ambient, 0.0f, 1.0f);
    ImGui::SliderFloat3("Diffuse", diffuse, 0.0f, 1.0f);
    ImGui::SliderFloat3("Specular", specular, 0.0f, 1.0f);
    ImGui::SliderFloat("Specular Shininess", &shininess, 0.0f, 1.0f);

    ImGui::EndChild();
}

void DebugUi::lightSettings(int selected)
{
    ImGui::BeginChild("Light settings", ImVec2(0, -(ImGui::GetFrameHeightWithSpacing() + 80)));
    ImGui::Text("Light settings");

    if (ImGui::Button("Remove Light", ImVec2(100, 0))) {
        m_scene->deleteDirectionalLight(selected);
    }

    static float ambient[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
    static float diffuse[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
    static float specular[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
    //glm::vec4 ambientVec;
    //glm::vec4 diffuseVec;
    //glm::vec4 specularVec;
    float shininess;

    ImGui::SliderFloat3("Ambient Light", ambient, 0.0f, 1.0f);
    ImGui::SliderFloat3("Diffuse Light", diffuse, 0.0f, 1.0f);
    ImGui::SliderFloat3("Specular Light", specular, 0.0f, 1.0f);
    ImGui::SliderFloat("Specular Shininess", &shininess, 0.0f, 1.0f);

    ImGui::EndChild();
}

void DebugUi::meshSettings(int selected)
{
    ImGui::BeginChild("Mesh panel", ImVec2(0, -(ImGui::GetFrameHeightWithSpacing() + 80))); // Leave room for 1 line below us
    
    ImGui::Text("Mesh settings");

    std::vector<MeshObject*>* meshList = m_scene->getMeshList();

    if (meshList->size() == 0 && selected >= meshList->size()) {
        ImGui::EndChild();
        return;
    }

    RepeaterState* state;
    if (meshList->at(selected)->type == MeshType::ModelType)
        state = meshList->at(selected)->model->getMeshList()->at(0)->getState();
    else
        state = meshList->at(selected)->mesh->getState();

    // Load current values
    if (m_scene->getMeshPointer() >= 0 && m_scene->getMeshPointer() < state->modified->size()) {
        m_meshState.scaleX = state->modified->at(m_scene->getMeshPointer())->transformations->scaleX;
        m_meshState.scaleY = state->modified->at(m_scene->getMeshPointer())->transformations->scaleY;
        m_meshState.scaleZ = state->modified->at(m_scene->getMeshPointer())->transformations->scaleZ;

        m_meshState.xOffset = state->modified->at(m_scene->getMeshPointer())->transformations->xOffset;
        m_meshState.yOffset = state->modified->at(m_scene->getMeshPointer())->transformations->yOffset;
        m_meshState.zOffset = state->modified->at(m_scene->getMeshPointer())->transformations->zOffset;

        m_meshState.paddingX = state->modified->at(m_scene->getMeshPointer())->transformations->paddingX;
        m_meshState.paddingY = state->modified->at(m_scene->getMeshPointer())->transformations->paddingY;
        m_meshState.paddingZ = state->modified->at(m_scene->getMeshPointer())->transformations->paddingZ;

        m_meshState.angle = state->modified->at(m_scene->getMeshPointer())->transformations->angle;
        m_meshState.xRotation = state->modified->at(m_scene->getMeshPointer())->transformations->xRotation;
        m_meshState.yRotation = state->modified->at(m_scene->getMeshPointer())->transformations->yRotation;
        m_meshState.zRotation = state->modified->at(m_scene->getMeshPointer())->transformations->zRotation;
    }
    else {
        std::cout << "modified mesh pointer error1!" << std::endl;
    }

    // Draw settings
    if (ImGui::CollapsingHeader("Scale")) {
        ImGui::SliderFloat("Scale X", &m_meshState.scaleX, 0.01f, 10.0f);
        ImGui::SliderFloat("Scale Y", &m_meshState.scaleY, 0.01f, 10.0f);
        ImGui::SliderFloat("Scale Z", &m_meshState.scaleZ, 0.01f, 10.0f);
    }
    
    if (ImGui::CollapsingHeader("Offset")) {
        ImGui::SliderFloat("Offset X", &m_meshState.xOffset, -5.0f, 5.0f);
        ImGui::SliderFloat("Offset Y", &m_meshState.yOffset, -5.0f, 5.0f);
        ImGui::SliderFloat("Offset Z", &m_meshState.zOffset, -5.0f, 5.0f);
    }

    if (ImGui::CollapsingHeader("Padding")) {
        ImGui::SliderFloat("Padding X", &m_meshState.paddingX, 0.0f, 10.0f);
        ImGui::SliderFloat("Padding Y", &m_meshState.paddingY, 0.0f, 10.0f);
        ImGui::SliderFloat("Padding Z", &m_meshState.paddingZ, 0.0f, 10.0f);
    }

    if (ImGui::CollapsingHeader("Rotation")) {
        ImGui::SliderFloat("Angle", &m_meshState.angle, 0.0f, 360.0f);
        ImGui::SliderFloat("Rotation X", &m_meshState.xRotation, 0.001f, 1.0f);
        ImGui::SliderFloat("Rotation Y", &m_meshState.yRotation, 0.001f, 1.0f);
        ImGui::SliderFloat("Rotation Z", &m_meshState.zRotation, 0.001f, 1.0f);
    }

    // Check if settings changed
    if (m_debounceCounter >= .1f) {
        if (m_scene->getMeshPointer() >= 0 && m_scene->getMeshPointer() < state->modified->size()) {
            if (m_meshState.scaleX != state->modified->at(m_scene->getMeshPointer())->transformations->scaleX ||
                m_meshState.scaleY != state->modified->at(m_scene->getMeshPointer())->transformations->scaleY ||
                m_meshState.scaleZ != state->modified->at(m_scene->getMeshPointer())->transformations->scaleZ ||
                
                m_meshState.xOffset != state->modified->at(m_scene->getMeshPointer())->transformations->xOffset ||
                m_meshState.yOffset != state->modified->at(m_scene->getMeshPointer())->transformations->yOffset ||
                m_meshState.zOffset != state->modified->at(m_scene->getMeshPointer())->transformations->zOffset ||

                m_meshState.paddingX != state->modified->at(m_scene->getMeshPointer())->transformations->paddingX ||
                m_meshState.paddingY != state->modified->at(m_scene->getMeshPointer())->transformations->paddingY ||
                m_meshState.paddingZ != state->modified->at(m_scene->getMeshPointer())->transformations->paddingZ ||

                m_meshState.angle != state->modified->at(m_scene->getMeshPointer())->transformations->angle ||
                m_meshState.xRotation != state->modified->at(m_scene->getMeshPointer())->transformations->xRotation ||
                m_meshState.yRotation != state->modified->at(m_scene->getMeshPointer())->transformations->yRotation ||
                m_meshState.zRotation != state->modified->at(m_scene->getMeshPointer())->transformations->zRotation
                ) {

                state->modified->at(m_scene->getMeshPointer())->transformations->scaleX = m_meshState.scaleX;
                state->modified->at(m_scene->getMeshPointer())->transformations->scaleY = m_meshState.scaleY;
                state->modified->at(m_scene->getMeshPointer())->transformations->scaleZ = m_meshState.scaleZ;

                state->modified->at(m_scene->getMeshPointer())->transformations->xOffset = m_meshState.xOffset;
                state->modified->at(m_scene->getMeshPointer())->transformations->yOffset = m_meshState.yOffset;
                state->modified->at(m_scene->getMeshPointer())->transformations->zOffset = m_meshState.zOffset;

                state->modified->at(m_scene->getMeshPointer())->transformations->paddingX = m_meshState.paddingX;
                state->modified->at(m_scene->getMeshPointer())->transformations->paddingY = m_meshState.paddingY;
                state->modified->at(m_scene->getMeshPointer())->transformations->paddingZ = m_meshState.paddingZ;

                state->modified->at(m_scene->getMeshPointer())->transformations->angle = m_meshState.angle;
                state->modified->at(m_scene->getMeshPointer())->transformations->xRotation = m_meshState.xRotation;
                state->modified->at(m_scene->getMeshPointer())->transformations->yRotation = m_meshState.yRotation;
                state->modified->at(m_scene->getMeshPointer())->transformations->zRotation = m_meshState.zRotation;

                std::cout << "Changed!" << std::endl;

                MeshObjectChange change;
                change.selectedMesh = selected;
                change.action = MeshObjectChangeAction::UpdateObject;
                m_scene->updateMeshObjects(change);

                m_debounceCounter = 0;
            }
        }
        else {
            std::cout << "modified mesh pointer error2!" << std::endl;
        }
    }

    ImGui::EndChild();
}

bool DebugUi::objectSettings(int selected)
{
    std::vector<MeshObject*>* meshList = m_scene->getMeshList();

    // Reset mesh pointer
    if (selected < meshList->size()) {
        if (!meshList->at(selected)->selected) {
            MeshObjectChange change;
            change.action = MeshObjectChangeAction::ResetMeshPointer;
            m_scene->updateMeshObjects(change);
        }
    }

    ImGui::BeginChild("item view", ImVec2(0, -(ImGui::GetFrameHeightWithSpacing() + 80))); // Leave room for 1 line below us and mesh edit view

    if (meshList->size() > selected) {
        ImGui::Text(meshList->at(selected)->name.c_str(), selected);

        if (ImGui::Button("Remove Object", ImVec2(100, 0))) {
            if (meshList->at(selected)->type == MeshType::ModelType)
                m_scene->deleteModel(selected);
            else
                m_scene->deleteObject(selected);

            ImGui::EndChild();
            return false;
        }

        ImGui::Separator();

        // Set selected
        meshList->at(selected)->selected = true;

        // Load current values
        RepeaterState* state;
        if (meshList->at(selected)->type == MeshType::ModelType)
            state = meshList->at(selected)->model->getMeshList()->at(0)->getState();
        else
            state = meshList->at(selected)->mesh->getState();
        m_planeState->instanced = state->instanced;
        m_planeState->columnCount = state->columnCount;
        m_planeState->rowCount = state->rowCount;
        m_planeState->stackCount = state->stackCount;
        m_planeState->transformations->scaleX = state->transformations->scaleX;
        m_planeState->transformations->scaleY = state->transformations->scaleY;
        m_planeState->transformations->scaleZ = state->transformations->scaleZ;
        m_planeState->transformations->paddingX = state->transformations->paddingX;
        m_planeState->transformations->paddingY = state->transformations->paddingY;
        m_planeState->transformations->paddingZ = state->transformations->paddingZ;
        m_planeState->transformations->xOffset = state->transformations->xOffset;
        m_planeState->transformations->yOffset = state->transformations->yOffset;
        m_planeState->transformations->zOffset = state->transformations->zOffset;

        m_planeState->transformations->angle = state->transformations->angle;
        m_planeState->transformations->xRotation = state->transformations->xRotation;
        m_planeState->transformations->yRotation = state->transformations->yRotation;
        m_planeState->transformations->zRotation = state->transformations->zRotation;

        // Set current values and draw settings
        ImGui::Checkbox("Instanced", &m_planeState->instanced);
        if (ImGui::CollapsingHeader("Repeater")) {
            ImGui::SliderInt("Columns", &m_planeState->columnCount, 1, 100);
            ImGui::SliderInt("Rows", &m_planeState->rowCount, 1, 100);
            ImGui::SliderInt("Stacks", &m_planeState->stackCount, 1, 100);
        }

        //ImGui::Separator();
        if (ImGui::CollapsingHeader("Scale")) {
            ImGui::SliderFloat("Scale X", &m_planeState->transformations->scaleX, 0.01f, 10.0f);
            ImGui::SliderFloat("Scale Y", &m_planeState->transformations->scaleY, 0.01f, 10.0f);
            ImGui::SliderFloat("Scale Z", &m_planeState->transformations->scaleZ, 0.01f, 10.0f);
        }

        //ImGui::Separator();
        if (ImGui::CollapsingHeader("Padding")) {
            ImGui::SliderFloat("Padding X", &m_planeState->transformations->paddingX, 0.0f, 10.0f);
            ImGui::SliderFloat("Padding Y", &m_planeState->transformations->paddingY, 0.0f, 10.0f);
            ImGui::SliderFloat("Padding Z", &m_planeState->transformations->paddingZ, 0.0f, 10.0f);
        }

        //ImGui::Separator();
        if (ImGui::CollapsingHeader("Offset")) {
            ImGui::SliderFloat("X offset", &m_planeState->transformations->xOffset, -5.0f, 5.0f);
            ImGui::SliderFloat("Y offset", &m_planeState->transformations->yOffset, -5.0f, 5.0f);
            ImGui::SliderFloat("Z offset", &m_planeState->transformations->zOffset, -5.0f, 5.0f);
        }

        //ImGui::Separator();
        if (ImGui::CollapsingHeader("Rotation")) {
            ImGui::SliderFloat("Rotation angle", &m_planeState->transformations->angle, 0.0f, 360.0f);
            ImGui::SliderFloat("X Rotation", &m_planeState->transformations->xRotation, 0.001f, 1.0f);
            ImGui::SliderFloat("Y Rotation", &m_planeState->transformations->yRotation, 0.001f, 1.0f);
            ImGui::SliderFloat("Z Rotation", &m_planeState->transformations->zRotation, 0.001f, 1.0f);
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
                m_planeState->transformations->scaleX != state->transformations->scaleX ||
                m_planeState->transformations->scaleY != state->transformations->scaleY ||
                m_planeState->transformations->scaleZ != state->transformations->scaleZ ||
                m_planeState->transformations->paddingX != state->transformations->paddingX ||
                m_planeState->transformations->paddingY != state->transformations->paddingY ||
                m_planeState->transformations->paddingZ != state->transformations->paddingZ ||
                m_planeState->transformations->xOffset != state->transformations->xOffset ||
                m_planeState->transformations->yOffset != state->transformations->yOffset ||
                m_planeState->transformations->zOffset != state->transformations->zOffset ||
                m_planeState->transformations->angle != state->transformations->angle ||
                m_planeState->transformations->xRotation != state->transformations->xRotation ||
                m_planeState->transformations->yRotation != state->transformations->yRotation ||
                m_planeState->transformations->zRotation != state->transformations->zRotation
                ) {
                state->columnCount = m_planeState->columnCount;
                state->rowCount = m_planeState->rowCount;
                state->stackCount = m_planeState->stackCount;
                state->transformations->scaleX = m_planeState->transformations->scaleX;
                state->transformations->scaleY = m_planeState->transformations->scaleY;
                state->transformations->scaleZ = m_planeState->transformations->scaleZ;
                state->transformations->paddingX = m_planeState->transformations->paddingX;
                state->transformations->paddingY = m_planeState->transformations->paddingY;
                state->transformations->paddingZ = m_planeState->transformations->paddingZ;
                state->transformations->xOffset = m_planeState->transformations->xOffset;
                state->transformations->yOffset = m_planeState->transformations->yOffset;
                state->transformations->zOffset = m_planeState->transformations->zOffset;
                state->transformations->angle = m_planeState->transformations->angle;
                state->transformations->xRotation = m_planeState->transformations->xRotation;
                state->transformations->yRotation = m_planeState->transformations->yRotation;
                state->transformations->zRotation = m_planeState->transformations->zRotation;

                std::cout << "Changed!" << std::endl;

                MeshObjectChange change;
                change.selectedMesh = selected;
                change.action = MeshObjectChangeAction::UpdateObject;
                m_scene->updateMeshObjects(change);

                m_debounceCounter = 0;
            }
        }
    }

    ImGui::EndChild();

    return true;
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

        if (ImGui::Button("Add PointLight", ImVec2(100, 0)))
            m_scene->addPointLight();

        if (ImGui::Button("Add Directional Light", ImVec2(100, 0)))
            m_scene->addDirectionalLight();

        if (ImGui::Button("Add Spot Light", ImVec2(100, 0)))
            m_scene->addSpotLight();

        if (ImGui::Button("Add Model", ImVec2(100, 0)))
            m_scene->addModel();

        //if (ImGui::Button("Add Reflect Cube", ImVec2(100, 0)))
        //    m_scene->addReflectingCube();

        // Unselect all first
        std::vector<MeshObject*> meshList = *m_scene->getMeshList();
        for (int i = 0; i < meshList.size(); i++) {
            meshList.at(i)->selected = false;
        }

        // Mesh object selector
        static int selected = 0;
        {
            ImGui::BeginChild("left pane", ImVec2(150, 0), true);
            for (int i = 0; i < meshList.size(); i++)
            {
                if (ImGui::Selectable(meshList.at(i)->name.c_str(), selected == i))
                    selected = i;
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();

        // Update selected object variable
        m_selected = selected;

        ImGui::BeginGroup();

        // Begin tab group
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {

            // If reflecting light type, show light and material settings
            if (meshList.size() != 0 && selected < meshList.size()) {
                //if (meshList.at(selected)->type == MeshType::ReflectCubeType) {
                    if (ImGui::BeginTabItem("Material")) {
                        materialSettings(selected);
                        ImGui::EndTabItem();
                    }
                //}
                if (meshList.at(selected)->type == MeshType::DirectionalLightType ||
                    meshList.at(selected)->type == MeshType::PointLightType ||
                    meshList.at(selected)->type == MeshType::SpotLightType) {
                    if (ImGui::BeginTabItem("Light")) {
                        lightSettings(selected);
                        ImGui::EndTabItem();
                    }
                }
            }

            // Mesh settings
            if (ImGui::BeginTabItem("Mesh")) {
                meshSettings(selected);
                ImGui::EndTabItem();
            }

            // Repeater settings
            if (ImGui::BeginTabItem("Repeater")) {
                if (!objectSettings(selected)) {
                    ImGui::EndTabItem();
                    ImGui::EndTabBar();
                    ImGui::EndGroup();
                    ImGui::End();
                    return;
                }
                ImGui::EndTabItem();
            }

            // Mesh position info inside an instance
            {
                std::vector<MeshObject*>* meshList = m_scene->getMeshList();
                if (meshList->size() > selected) {
                    ImGui::BeginChild("Rightmost pane", ImVec2(0, 0), true);

                    if (ImGui::Button("Remove Mesh", ImVec2(100, 0))) {
                        MeshObjectChange change;
                        change.selectedMesh = selected;
                        change.action = MeshObjectChangeAction::DeleteInstancedMesh;
                        m_scene->updateMeshObjects(change);
                    }
                    ImGui::SameLine();

                    RepeaterState* state;
                    if (meshList->at(selected)->type == MeshType::ModelType)
                        state = meshList->at(selected)->model->getMeshList()->at(0)->getState();
                    else
                        state = meshList->at(selected)->mesh->getState();
                    ImGui::Text("Mesh pointer: %d", state->position->meshPointer);
                    ImGui::Text("Stack position:  %d", state->position->stackPosition);
                    ImGui::Text("Row position:    %d", state->position->rowPosition);
                    ImGui::Text("Column position: %d", state->position->columnPosition);

                    ImGui::EndChild();
                }
            }
            
            ImGui::EndTabBar();
        }
    }
    ImGui::EndGroup();
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