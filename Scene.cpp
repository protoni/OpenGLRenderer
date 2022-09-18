#include "Scene.h"
#include "stb_image.h"
#include "DebugMacros.h"


Scene::Scene(Camera *camera, ScreenSettings* screenSettings) :
    m_camera(camera), m_screenSettings(screenSettings), m_meshPointer(0)
{
    // Create and build shaders
    //m_ourShaderInstanced = new Shader("./shaderInstanced.vs", "./shader.fs");
    m_ourShaderInstanced = new Shader("./lightMeshShaderInstanced.vs", "./lightMeshShader.fs");
    m_ourShader = new Shader("./shader.vs", "./shader.fs");
    m_lightShader = new Shader("./shader.vs", "./lightShader.fs");
    m_lightMeshShader = new Shader("./lightMeshShader.vs", "./lightMeshShader.fs");
    m_modelLoadingShader = new Shader("./modelLoading.vs", "./modelLoading.fs");
    
    // Load texture
    m_container_texture = new Texture("container2.png");
    m_container_texture_specular = new Texture("container2_specular.png");

    // Create mesh vector
    m_meshList = new std::vector<MeshObject*>;

    // Create a mesh list handler
    m_meshListHandler = new MeshListHandler(m_meshList);
}

Scene::~Scene()
{
    if (m_meshList) {
        delete m_meshList;
        m_meshList = NULL;
    }

    if (m_container_texture) {
        delete m_container_texture;
        m_container_texture = NULL;
    }

    if (m_container_texture_specular) {
        delete m_container_texture_specular;
        m_container_texture_specular = NULL;
    }

    if (m_ourShaderInstanced) {
        delete m_ourShaderInstanced;
        m_ourShaderInstanced = NULL;
    }

    if (m_ourShader) {
        delete m_ourShader;
        m_ourShader = NULL;
    }

    if (m_lightShader) {
        delete m_lightShader;
        m_lightShader = NULL;
    }

    if (m_lightMeshShader) {
        delete m_lightMeshShader;
        m_lightMeshShader = NULL;
    }

    if (m_modelLoadingShader) {
        delete m_modelLoadingShader;
        m_modelLoadingShader = NULL;
    }

    if (m_meshListHandler) {
        delete m_meshListHandler;
        m_meshListHandler = NULL;
    }
}

void Scene::updateMeshShader(bool instanced, int idx)
{
    if (m_meshList->at(idx)->type == MeshType::ModelType) {
        m_meshList->at(idx)->model->setInstanced(instanced);
    }
    else {
        if (instanced) {
            m_meshList->at(idx)->mesh->setShader(m_ourShaderInstanced);
        }
        else {
            m_meshList->at(idx)->mesh->setShader(m_ourShader);
        }

        m_meshList->at(idx)->mesh->setInstanced(instanced);
    }
}

void Scene::addCube()
{
    Cube* cube = new Cube(m_lightMeshShader, false, false, true);
    MeshObject* object = new MeshObject();
    MeshLights* light = new MeshLights();
    MaterialBase* material = new MaterialDefault();

    object->mesh = cube;
    object->name = std::string("Cube_") + std::to_string(m_meshList->size());
    object->type = MeshType::ReflectCubeType;
    object->light = light;
    object->material = material;
    m_meshList->push_back(object);
}

void Scene::addPlane()
{
    Plane* plane = new Plane(m_lightMeshShader, false, false, true);
    MeshObject* object = new MeshObject();
    MeshLights* light = new MeshLights();
    MaterialBase* material = new MaterialDefault();
    object->mesh = plane;
    object->name = std::string("Plane_") + std::to_string(m_meshList->size());
    object->type = MeshType::ReflectCubeType;
    object->light = light;
    object->material = material;
    m_meshList->push_back(object);
}

void Scene::addTriangle()
{
    Triangle* triangle = new Triangle(m_ourShader, false);
    MeshObject* object = new MeshObject();
    object->mesh = triangle;
    object->name = std::string("Triangle_") + std::to_string(m_meshList->size());
    m_meshList->push_back(object);
}

void Scene::addSphere()
{
    Sphere* sphere = new Sphere(m_ourShader, false);
    MeshObject* object = new MeshObject();
    object->mesh = sphere;
    object->name = std::string("Sphere_") + std::to_string(m_meshList->size());
    m_meshList->push_back(object);
}

void Scene::addCustom()
{
    Custom* custom = new Custom(m_ourShader, false);
    MeshObject* object = new MeshObject();
    object->mesh = custom;
    object->name = std::string("Custom_") + std::to_string(m_meshList->size());
    m_meshList->push_back(object);
}

void Scene::addReflectingCube()
{
    Cube* cube = new Cube(m_lightMeshShader, false, false, true);
    MeshObject* object = new MeshObject();
    MeshLights* light = new MeshLights();
    MaterialBase* material = new MaterialDefault();

    object->mesh = cube;
    object->name = std::string("ReflectingCube_") + std::to_string(m_meshList->size());
    object->type = MeshType::ReflectCubeType;
    object->light = light;
    object->material = material;
    m_meshList->push_back(object);
}

void Scene::addLight()
{
    Cube* cube = new Cube(m_lightShader, false, true, true);
    MeshObject* object = new MeshObject();
    object->mesh = cube;
    object->name = std::string("Light_") + std::to_string(m_meshList->size());
    object->type = MeshType::LightType;
    
    m_meshList->push_back(object);
    m_pointLights.push_back(object);
}

void Scene::addDirectionalLight()
{
    Cube* cube = new Cube(m_lightShader, false, true, true);
    MeshObject* object = new MeshObject();
    object->mesh = cube;
    object->name = std::string("DirectionalLight_") + std::to_string(m_meshList->size());
    object->type = MeshType::DirectionalLightType;
    
    m_meshList->push_back(object);
    m_directionalLights.push_back(object);
}

void Scene::addSpotLight()
{
    Cube* cube = new Cube(m_lightShader, false, true, true);
    MeshObject* object = new MeshObject();
    object->mesh = cube;
    object->name = std::string("Spotlight_") + std::to_string(m_meshList->size());
    object->type = MeshType::SpotLightType;

    m_meshList->push_back(object);
    m_spotLights.push_back(object);
}

void Scene::addModel()
{
    Model* model = new Model(m_lightMeshShader);
    model->LoadModel("Models/backpack/backpack.obj");
    MeshObject* object = new MeshObject();
    object->model = model;
    object->name = std::string("Model_") + std::to_string(m_meshList->size());
    object->type = MeshType::ModelType;

    m_meshList->push_back(object);
}

bool Scene::updateMeshObjects(MeshObjectChange& change)
{
    bool ret = true;

    switch (change.action)
    {

    case MeshObjectChangeAction::UpdateObject:
        m_meshListHandler->updateObjectMesh(change.selectedMesh);
        break;
    
    case MeshObjectChangeAction::ResetMeshPointer:
        m_meshListHandler->resetMeshPointer();
        break;
        
    case MeshObjectChangeAction::UpdateMeshPointer:
        m_meshListHandler->updateMeshPointer(change.direction, change.multiselect);
        break;

    case MeshObjectChangeAction::DeleteInstancedMesh:
        m_meshListHandler->deleteInstancedMesh(change.selectedMesh);
        break; 

    case MeshObjectChangeAction::SetMultiPickMode:
        m_meshListHandler->setMultiPickMode(change.multiPick);
        break; 

    case MeshObjectChangeAction::MultiPick:
        m_meshListHandler->multiPick();
        break;

    default:
        std::cout << "Unknown mesh object move action!" << std::endl;
        ret = false;
        break;
    }

    return ret;
}

int Scene::getObjectCount()
{
    return m_meshListHandler->getObjectCount();
}

int Scene::getTriangleCount()
{
    return m_meshListHandler->getTriangleCount();
}

void Scene::deleteDirectionalLight(int selected)
{
    if (m_meshList->at(selected)->type == MeshType::DirectionalLightType) {
        m_directionalLights.clear(); // TODO: when multiple lights implemented, don't delete all
        std::cout << "delete directional light!" << std::endl;
    }
    else if (m_meshList->at(selected)->type == MeshType::LightType) {
        m_lightPos = glm::vec3(-0.0f, -0.0f, -0.0f);
    }
}

std::vector<MeshObject*>* Scene::getMeshList()
{
    return m_meshList;
}

void Scene::updateMeshMaterial(int selected, const std::string& newMaterial)
{
    // Clear old material
    delete m_meshList->at(selected)->material;

    MaterialBase* material;
    MaterialCollection materialCollection;

    // Set new material
    if (newMaterial.compare(materialCollection.materialDefault.name) == 0) {
        material = new MaterialDefault();
    }
    else if (newMaterial.compare(materialCollection.materialEmerald.name) == 0) {
        material = new MaterialEmerald();
    }
    else if (newMaterial.compare(materialCollection.materialSilver.name) == 0) {
        material = new MaterialSilver();
    }
    else if (newMaterial.compare(materialCollection.materialGreenRubber.name) == 0) {
        material = new MaterialGreenRubber();
    }
    else if (newMaterial.compare(materialCollection.materialYellowRubber.name) == 0) {
        material = new MaterialYellowRubber();
    }
    else
        material = new MaterialDefault();

    m_meshList->at(selected)->material = material;
}

void Scene::renderDirectionalLight(int idx, Shader* shader)
{
    // Return if max point light count
    if (idx >= 40)
        return;

    // Get light's mesh state
    RepeaterState* state;
    if (m_pointLights.at(idx)->type == MeshType::ModelType)
        state = m_pointLights.at(idx)->model->getMeshList()->at(0)->getState();
    else
        state = m_pointLights.at(idx)->mesh->getState();

    // Get light position
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    if (state->modified->size() > 0) {
        pos.x = state->modified->at(0)->transformations->xPos;
        pos.y = state->modified->at(0)->transformations->yPos;
        pos.z = state->modified->at(0)->transformations->zPos;
    }

    // Create uniform array location name
    std::string prefix = "dirLights[";
    std::string idxStr = std::to_string(idx);


    // Set light position
   std::string name = prefix + idxStr + "].direction";
   // m_ourShaderInstanced->setVec3(name, pos);

    // Set light direction
    //name = prefix + idxStr + "].direction";
    m_ourShaderInstanced->setVec3(name, pos);

    // Set light values
    name = prefix + idxStr + "].ambient";
    m_ourShaderInstanced->setVec3(name, glm::vec3(0.05f, 0.05f, 0.05f));

    name = prefix + idxStr + "].diffuse";
    m_ourShaderInstanced->setVec3(name, glm::vec3(0.4f, 0.4f, 0.4f));

    name = prefix + idxStr + "].specular";
    m_ourShaderInstanced->setVec3(name, glm::vec3(0.5f, 0.5f, 0.5f));

}

void Scene::renderSpotLight(int idx, Shader* shader)
{
    // Return if max point light count
    if (idx >= 40)
        return;

    // Get light's mesh state
    RepeaterState* state;
    if (m_pointLights.at(idx)->type == MeshType::ModelType)
        state = m_pointLights.at(idx)->model->getMeshList()->at(0)->getState();
    else
        state = m_pointLights.at(idx)->mesh->getState();

    // Get light position
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    if (state->modified->size() > 0) {
        pos.x = state->modified->at(0)->transformations->xPos;
        pos.y = state->modified->at(0)->transformations->yPos;
        pos.z = state->modified->at(0)->transformations->zPos;
    }

    // Create uniform array location name
    std::string prefix = "spotLights[";
    std::string idxStr = std::to_string(idx);
    

    // Set light position
    std::string name = prefix + idxStr + "].position";
    m_ourShaderInstanced->setVec3(name, pos);

    // Set light direction
    name = prefix + idxStr + "].direction";
    m_ourShaderInstanced->setVec3(name, glm::vec3(1.0f, 0.0f, 0.0f));


    // Set light values
    name = prefix + idxStr + "].ambient";
    m_ourShaderInstanced->setVec3(name, glm::vec3(0.0f, 0.0f, 0.0f));

    name = prefix + idxStr + "].diffuse";
    m_ourShaderInstanced->setVec3(name, glm::vec3(1.0f, 1.0f, 1.0f));

    name = prefix + idxStr + "].specular";
    m_ourShaderInstanced->setVec3(name, glm::vec3(1.0f, 1.0f, 1.0f));


    // Set fade-out values
    name = prefix + idxStr + "].constant";
    m_ourShaderInstanced->setFloat(name, 1.0f);

    name = prefix + idxStr + "].linear";
    m_ourShaderInstanced->setFloat(name, 0.09f);

    name = prefix + idxStr + "].quadratic";
    m_ourShaderInstanced->setFloat(name, 0.032f);


    // Set spot area values
    name = prefix + idxStr + "].cutOff";
    m_ourShaderInstanced->setFloat(name, glm::cos(glm::radians(12.5f)));

    name = prefix + idxStr + "].outerCutOff";
    m_ourShaderInstanced->setFloat(name, glm::cos(glm::radians(15.0f)));
}

void Scene::renderPointLight(int idx, Shader* shader)
{
    // Return if max point light count
    if (idx >= 40) 
        return;

    // Get light's mesh state
    RepeaterState* state;
    if (m_pointLights.at(idx)->type == MeshType::ModelType)
        state = m_pointLights.at(idx)->model->getMeshList()->at(0)->getState();
    else
        state = m_pointLights.at(idx)->mesh->getState();

    // Get light position
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    if (state->modified->size() > 0) {
        pos.x = state->modified->at(0)->transformations->xPos;
        pos.y = state->modified->at(0)->transformations->yPos;
        pos.z = state->modified->at(0)->transformations->zPos;
    }

    // Create uniform array location name
    std::string prefix = "pointLights[";
    std::string idxStr = std::to_string(idx);
    std::string name = prefix + idxStr + "].position";

    // Set light position
    shader->setVec3(name, -pos);

    name = prefix + idxStr + "].ambient";
    shader->setVec3(name, glm::vec3(0.05f, 0.05f, 0.05f));

    name = prefix + idxStr + "].diffuse";
    shader->setVec3(name, glm::vec3(0.8f, 0.8f, 0.8f));

    name = prefix + idxStr + "].specular";
    shader->setVec3(name, glm::vec3(1.0f, 1.0f, 1.0f));


    // Set light fade-out values
    name = prefix + idxStr + "].constant";
    shader->setFloat(name, 1.0f);

    name = prefix + idxStr + "].linear";
    shader->setFloat(name, 0.09f);

    name = prefix + idxStr + "].quadratic";
    shader->setFloat(name, 0.032f);
}

void Scene::drawModel(int idx, glm::mat4& projection, glm::mat4& view)
{
    Model* model = m_meshList->at(idx)->model;

    m_lightMeshShader->use();
    m_lightMeshShader->setMat4("projection", projection);
    m_lightMeshShader->setMat4("view", view);

    // Set viewer position
    m_lightMeshShader->setVec3("viewPos", m_camera->Position);

    // Directional lights
    m_lightMeshShader->setInt("dirLightCount", m_directionalLights.size());
    for (int i = 0; i < m_directionalLights.size(); i++) {
        renderDirectionalLight(i, m_lightMeshShader);
    }

    // Point lights
    m_lightMeshShader->setInt("pointLightCount", m_pointLights.size());
    for (int i = 0; i < m_pointLights.size(); i++) {
        renderPointLight(i, m_lightMeshShader);
    }

    // Spotlights
    m_lightMeshShader->setInt("spotLightCount", m_spotLights.size());
    for (int i = 0; i < m_spotLights.size(); i++) {
        renderSpotLight(i, m_lightMeshShader);
    }

    // Set material
    if (m_meshList->at(idx)->material) {
        m_lightMeshShader->setVec3("material.ambient", m_meshList->at(idx)->material->ambient);
        m_lightMeshShader->setVec3("material.diffuse", m_meshList->at(idx)->material->diffuse);
        m_lightMeshShader->setVec3("material.specular", m_meshList->at(idx)->material->specular);
        m_lightMeshShader->setFloat("material.shininess", m_meshList->at(idx)->material->shininess);
    }
    else {
        std::cout << "Error! Mesh doesn't have material. Using defaults" << std::endl;
        m_lightMeshShader->setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        m_lightMeshShader->setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        m_lightMeshShader->setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        m_lightMeshShader->setFloat("material.shininess", 32);
    
    }

    model->RenderModel();
}

void Scene::draw(int idx, glm::mat4& projection, glm::mat4& view)
{
    RepeaterState* state = m_meshList->at(idx)->mesh->getState();
    if (state->instanced) {
        m_ourShaderInstanced->use();
        m_ourShaderInstanced->setMat4("projection", projection);
        m_ourShaderInstanced->setMat4("view", view);
        m_ourShaderInstanced->setInt("selectedMesh", m_meshPointer);
        m_ourShaderInstanced->setInt("selectedInstance", m_meshList->at(idx)->selected);

        m_meshListHandler->highlightChanged();

        m_ourShaderInstanced->setVec3("viewPos", m_camera->Position);

        // Directional lights
        m_ourShaderInstanced->setInt("dirLightCount", m_directionalLights.size());
        for (int i = 0; i < m_directionalLights.size(); i++) {
            renderDirectionalLight(i, m_ourShaderInstanced);
        }

        // Point lights
        m_ourShaderInstanced->setInt("pointLightCount", m_pointLights.size());
        for (int i = 0; i < m_pointLights.size(); i++) {
            renderPointLight(i, m_ourShaderInstanced);
        }

        // Spotlights
        m_ourShaderInstanced->setInt("spotLightCount", m_spotLights.size());
        for (int i = 0; i < m_spotLights.size(); i++) {
            renderSpotLight(i, m_ourShaderInstanced);
        }

        // Set material
        if (m_meshList->at(idx)->material) {
            m_ourShaderInstanced->setVec3("material.ambient", m_meshList->at(idx)->material->ambient);
            m_ourShaderInstanced->setVec3("material.diffuse", m_meshList->at(idx)->material->diffuse);
            m_ourShaderInstanced->setVec3("material.specular", m_meshList->at(idx)->material->specular);
            m_ourShaderInstanced->setFloat("material.shininess", m_meshList->at(idx)->material->shininess);
        }
        else {
            std::cout << "Error! Mesh doesn't have material. Using defaults" << std::endl;
            m_ourShaderInstanced->setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
            m_ourShaderInstanced->setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
            m_ourShaderInstanced->setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
            m_ourShaderInstanced->setFloat("material.shininess", 32);
            
        }
    }
    else {
        if (m_meshList->at(idx)->type == MeshType::ReflectCubeType) {
            m_lightMeshShader->use();
            m_lightMeshShader->setMat4("projection", projection);
            m_lightMeshShader->setMat4("view", view);
            m_lightMeshShader->setVec3("viewPos", m_camera->Position);

            // Set lights
            if (m_meshList->at(idx)->light) {
                m_lightMeshShader->setVec3("light.ambient", m_meshList->at(idx)->light->ambient);
                m_lightMeshShader->setVec3("light.diffuse", m_meshList->at(idx)->light->diffuse);
                m_lightMeshShader->setVec3("light.specular", m_meshList->at(idx)->light->specular);
            }
            else {
                std::cout << "Error! Mesh doesn't have lights. Using defaults" << std::endl;
                m_lightMeshShader->setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
                m_lightMeshShader->setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
                m_lightMeshShader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            }

            // Set material
            if (m_meshList->at(idx)->material) {
                m_lightMeshShader->setVec3("material.ambient", m_meshList->at(idx)->material->ambient);
                m_lightMeshShader->setVec3("material.diffuse", m_meshList->at(idx)->material->diffuse);
                m_lightMeshShader->setVec3("material.specular", m_meshList->at(idx)->material->specular);
                m_lightMeshShader->setFloat("material.shininess", m_meshList->at(idx)->material->shininess);
            }
            else {
                std::cout << "Error! Mesh doesn't have material. Using defaults" << std::endl;
                m_lightMeshShader->setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
                m_lightMeshShader->setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
                m_lightMeshShader->setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
                m_lightMeshShader->setFloat("material.shininess", 32);
            }
        }
        else {
            m_ourShader->use();
            m_ourShader->setMat4("projection", projection);
            m_ourShader->setMat4("view", view);

            if (m_meshList->at(idx)->selected)
                m_ourShader->setVec4("selectColor", glm::vec4(0.2, 0.0, 0.0, 0.5));
            else
                m_ourShader->setVec4("selectColor", glm::vec4(0.0, 0.0, 0.0, 0.0));
            
            
        }

        if (m_meshList->at(idx)->type == MeshType::LightType) {
            m_lightShader->use();
            m_lightShader->setMat4("projection", projection);
            m_lightShader->setMat4("view", view);

            // Update light position
            if (state->modified->size() > 0) {
                m_lightPos.x = state->modified->at(0)->transformations->xPos;
                m_lightPos.y = state->modified->at(0)->transformations->yPos;
                m_lightPos.z = state->modified->at(0)->transformations->zPos;
            }

            

        }

        // Update directional lights
        if (m_meshList->at(idx)->type == MeshType::DirectionalLightType) {
            m_lightShader->use();
            m_lightShader->setMat4("projection", projection);
            m_lightShader->setMat4("view", view);
        }

        // Update spot lights
        if (m_meshList->at(idx)->type == MeshType::SpotLightType) {
            m_lightShader->use();
            m_lightShader->setMat4("projection", projection);
            m_lightShader->setMat4("view", view);
        }
    }

    m_meshList->at(idx)->mesh->draw();
}

void Scene::renderScene()
{
    if (!m_ourShader || !m_ourShaderInstanced) {
        std::cout << "Scene::renderScene::shader error!" << std::endl;
        return;
    }

    
    m_lightMeshShader->setInt("material.diffuse", 0);
    m_container_texture->use(0);
    
    m_lightMeshShader->setInt("material.specular", 1);
    m_container_texture_specular->use(1);
    
    glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), (float)m_screenSettings->width / (float)m_screenSettings->height, 0.1f, 100.0f);
    glm::mat4 view = m_camera->GetViewMatrix();

    int lastDrawn = 0;
    for (int i = 0; i < m_meshList->size(); i++) {
        RepeaterState* state;
        if (m_meshList->at(i)->type == MeshType::ModelType)
            drawModel(i, projection, view);
        else
            draw(i, projection, view);
    }
}

void Scene::deleteObject(int idx)
{
    // Delete pointlights
    if (m_meshList->at(idx)->type == MeshType::LightType) {
        for (int i = 0; i < m_pointLights.size(); i++) {
            if (!m_pointLights.at(i)->name.compare(m_meshList->at(idx)->name))
            m_pointLights.erase(m_pointLights.begin() + i);
        }
    }

    // Delete directional lights
    if (m_meshList->at(idx)->type == MeshType::DirectionalLightType) {
        for (int i = 0; i < m_directionalLights.size(); i++) {
            if (!m_directionalLights.at(i)->name.compare(m_meshList->at(idx)->name))
                m_directionalLights.erase(m_directionalLights.begin() + i);
        }
    }

    // Delete spot lights
    if (m_meshList->at(idx)->type == MeshType::SpotLightType) {
        for (int i = 0; i < m_spotLights.size(); i++) {
            if (!m_spotLights.at(i)->name.compare(m_meshList->at(idx)->name))
                m_spotLights.erase(m_spotLights.begin() + i);
        }
    }
    
    // Clear individual deleted meshes inside an instance
    RepeaterState* state = m_meshList->at(idx)->mesh->getState();
    if (state->deleted) {
        delete state->deleted;
    }

    // Clear mesh pointer position
    if (state->position)
        delete state->position;

    // Clear mesh transformations
    if (state->transformations)
        delete state->transformations;

    // Cler modified mesh list
    if (state->modified) {
        //for (int i = 0; i < state->modified->size(); i++) {
        //    delete state->modified->at(i);
        //}

        // Clear old mesh transformations
        for (int i = 0; i < state->modified->size(); i++) {
            if (state->modified->at(i)) {
                if (state->modified->at(i)->position) {
                    delete state->modified->at(i)->position;
                    state->modified->at(i)->position = NULL;
                }
                if (state->modified->at(i)->transformations) {
                    delete state->modified->at(i)->transformations;
                    state->modified->at(i)->transformations = NULL;
                }
                if (state->modified->at(i)) {
                    delete state->modified->at(i);
                    state->modified->at(i) = NULL;
                }
            }
        }

        delete state->modified;
    }

    // Clear the mesh
    delete m_meshList->at(idx)->mesh;

    // Clear lights
    if(m_meshList->at(idx)->light)
        delete m_meshList->at(idx)->light;

    // Clear material
    if(m_meshList->at(idx)->material)
        delete m_meshList->at(idx)->material;

    // Clear object
    delete m_meshList->at(idx);

    // Clear the object list
    m_meshList->erase(m_meshList->begin() + idx);
}

void Scene::deleteModel(int idx)
{
    std::vector<ModelMesh*> meshes = *m_meshList->at(idx)->model->getMeshList();

    for (int i = 0; i < meshes.size(); i++) {
        std::cout << "Deleting model mesh: " << i << std::endl;
        RepeaterState* state = meshes.at(i)->getState();

        if (state->deleted) {
            delete state->deleted;
        }

        // Clear mesh pointer position
        if (state->position)
            delete state->position;

        // Clear mesh transformations
        if (state->transformations)
            delete state->transformations;

        // Cler modified mesh list
        if (state->modified) {
            //for (int i = 0; i < state->modified->size(); i++) {
            //    delete state->modified->at(i);
            //}

            // Clear old mesh transformations
            for (int i = 0; i < state->modified->size(); i++) {
                if (state->modified->at(i)) {
                    if (state->modified->at(i)->position) {
                        delete state->modified->at(i)->position;
                        state->modified->at(i)->position = NULL;
                    }
                    if (state->modified->at(i)->transformations) {
                        delete state->modified->at(i)->transformations;
                        state->modified->at(i)->transformations = NULL;
                    }
                    if (state->modified->at(i)) {
                        delete state->modified->at(i);
                        state->modified->at(i) = NULL;
                    }
                }
            }

            delete state->modified;
        }
    }

    // Clear model
    delete m_meshList->at(idx)->model;

    // Clear model object
    delete m_meshList->at(idx);
    
}

void Scene::clean()
{
    std::cout << "Cleaning: " << m_meshList->size() << " objects!" << std::endl;

    for (int i = m_meshList->size() -1; i >= 0; i--) {
        if (m_meshList->at(i)->type == MeshType::ModelType)
            deleteModel(i);
        else
            deleteObject(i);
    }

    m_meshList->clear();
}