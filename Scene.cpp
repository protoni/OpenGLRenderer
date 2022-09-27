#include "Scene.h"
#include "stb_image.h"
#include "DebugMacros.h"


Scene::Scene(Camera *camera, ScreenSettings* screenSettings) :
    m_camera(camera), m_screenSettings(screenSettings)
{
    // Create and build shaders
    //m_ourShaderInstanced = new Shader("./shaderInstanced.vs", "./shader.fs");
    m_ourShaderInstanced = new Shader("./lightMeshShaderInstanced.vs", "./lightMeshShader.fs");
    m_ourShader = new Shader("./shader.vs", "./shader.fs");
    m_lightShader = new Shader("./shader.vs", "./lightShader.fs");
    m_lightMeshShader = new Shader("./lightMeshShader.vs", "./lightMeshShader.fs");
    m_modelLoadingShader = new Shader("./lightMeshShader.vs", "./lightMeshShader.fs");
    m_terrainShader = new Shader("./terrain.vs", "./terrain.fs");

    // Load texture
    m_container_texture = new Texture("Textures/dirt.png", true);
    m_container_texture_specular = new Texture("container2_specular.png", true);

    // Create mesh vector
    m_meshList = new std::vector<MeshObject*>;

    // Create a mesh list handler
    m_meshListHandler = new MeshListHandler(m_meshList);

    // Create a material handler
    m_materialHandler = new MaterialHandler();

    // Create a light handler to render all of the light types
    m_lightHandler = new LightHandler(
        m_pointLights,
        m_directionalLights,
        m_spotLights
    );

    // Create a mouse picker
    m_mousePicker = new MousePicker(m_camera);

    // Initiate physics
    m_physics = new Physics();

    // Create a physical ground plane and a pointlight for testing
    createDefaultScene();

    // Generate terrain
    m_terrain = new Terrain(0, 0);
    m_terrain2 = new Terrain(1, 0);
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

    if (m_materialHandler) {
        delete m_materialHandler;
        m_materialHandler = NULL;
    }

    if (m_lightHandler) {
        delete m_lightHandler;
        m_lightHandler = NULL;
    }

    if (m_mousePicker) {
        delete m_mousePicker;
        m_mousePicker = NULL;
    }

    if (m_physics) {
        delete m_physics;
        m_physics = NULL;
    }
    
    if (m_terrain) {
        delete m_terrain;
        m_terrain = NULL;
    }

    if (m_terrain2) {
        delete m_terrain2;
        m_terrain2 = NULL;
    }

    if (m_terrainShader) {
        delete m_terrainShader;
        m_terrainShader = NULL;
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

void Scene::addMeshObject(Repeater* mesh, MeshType type)
{
    MeshObject* object = new MeshObject();
    MeshLights* light = new MeshLights();
    MaterialBase* material = new MaterialDefault();
    object->mesh = mesh;
    object->light = light;
    object->material = material;
    object->type = type;

    switch (type)
    {
    case CubeType:
        object->name = std::string("Cube_") + std::to_string(m_meshList->size());
        object->lightsEnabled = true;
        break;
    case CustomType:
        object->name = std::string("Custom_") + std::to_string(m_meshList->size());
        break;
    case PlaneType:
        object->name = std::string("Plane_") + std::to_string(m_meshList->size());
        object->lightsEnabled = true;
        break;
    case SphereType:
        object->name = std::string("Sphere_") + std::to_string(m_meshList->size());
        break;
    case TriangleType:
        object->name = std::string("Triangle_") + std::to_string(m_meshList->size());
        break;
    case PointLightType:
        object->name = std::string("PointLight_") + std::to_string(m_meshList->size());
        m_lightHandler->addPointLight(object);
        break;
    case DirectionalLightType:
        object->name = std::string("DirectionalLight_") + std::to_string(m_meshList->size());
        m_lightHandler->addDirectionalLight(object);
        break;
    case ReflectCubeType:
        object->name = std::string("ReflectingCube_") + std::to_string(m_meshList->size());
        object->lightsEnabled = true;
        break;
    case SpotLightType:
        object->name = std::string("Spotlight_") + std::to_string(m_meshList->size());
        m_lightHandler->addSpotLight(object);
        break;
    case ModelType:
        break;
    case UnknownType:
        break;
    default:
        std::cout << "Can't add unknown object type to the scene!" << std::endl;
        return;
    }

    m_meshList->push_back(object);
}

void Scene::addCube()
{
    Cube* cube = new Cube(m_lightMeshShader, false, false, true);
    addMeshObject(cube, MeshType::CubeType);
}

void Scene::addPhysicsCube()
{
    Cube* cube = new Cube(m_lightMeshShader, false, false, true);
    addMeshObject(cube, MeshType::CubeType);
    m_meshList->at(m_meshList->size() - 1)->mesh->getState()->mass = 1.0;
}

void Scene::addPlane()
{
    Plane* plane = new Plane(m_lightMeshShader, false, false, true);
    addMeshObject(plane, MeshType::PlaneType);
}

void Scene::addTriangle()
{
    Triangle* triangle = new Triangle(m_ourShader, false);
    addMeshObject(triangle, MeshType::TriangleType);
}

void Scene::addSphere()
{
    Sphere* sphere = new Sphere(m_ourShader, false);
    addMeshObject(sphere, MeshType::SphereType);
}

void Scene::addCustom()
{
    Custom* custom = new Custom(m_ourShader, false);
    addMeshObject(custom, MeshType::CustomType);
}

void Scene::addReflectingCube()
{
    Cube* cube = new Cube(m_lightMeshShader, false, false, true);
    addMeshObject(cube, MeshType::ReflectCubeType);
}

void Scene::addPointLight()
{
    Cube* cube = new Cube(m_lightShader, false, true, true);
    addMeshObject(cube, MeshType::PointLightType);
}

void Scene::addDirectionalLight()
{
    Cube* cube = new Cube(m_lightShader, false, true, true);
    addMeshObject(cube, MeshType::DirectionalLightType);
}

void Scene::addSpotLight()
{
    Cube* cube = new Cube(m_lightShader, false, true, true);
    addMeshObject(cube, MeshType::SpotLightType);
}

void Scene::addModel()
{
    Model* model = new Model(m_lightMeshShader, m_physics, m_mousePicker, m_camera);
    model->LoadModel("Models/backpack/backpack.obj");
    model->setMass(1.0f, false);
    MeshObject* object = new MeshObject();
    MaterialBase* material = new MaterialDefault();
    object->model = model;
    object->name = std::string("Model_") + std::to_string(m_meshList->size());
    object->type = MeshType::ModelType;
    object->material = material;
    
    m_meshList->push_back(object);
}

void Scene::addModel2()
{
    Model* model = new Model(m_lightMeshShader, m_physics, m_mousePicker, m_camera);
    model->LoadModel("Models/trollHotel/troll_hotel01.obj");
    model->setMass(1.0f, true);
    MeshObject* object = new MeshObject();
    MaterialBase* material = new MaterialDefault();
    object->model = model;
    object->name = std::string("Model_") + std::to_string(m_meshList->size());
    object->type = MeshType::ModelType;
    object->material = material;

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
    return m_meshListHandler->getTriangleCount() + (m_terrain->getIndiceCount() / 3);
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

int Scene::getMeshPointer()
{
    return m_meshListHandler->getMeshPointer();
}

glm::vec3& Scene::getCameraPosition()
{
    return m_camera->Position;
}

glm::vec2& Scene::getCursorWorldPos()
{
    return m_camera->getMouseWorldPos();
}

bool Scene::getPhysicsDebugMode()
{
    return m_physics->getDebugModeOn();
}

void Scene::setPhysicsDebugMode(bool state)
{
    m_physics->setDebugModeOn(state);
}

void Scene::addGround()
{
    addPlane();
    RepeaterState* state = m_meshList->at(m_meshList->size() - 1)->mesh->getState();

    // Set size
    state->columnCount = 20;
    state->rowCount = 20;

    // Set position
    state->transformations->xOffset = -1.0f;
    state->transformations->yOffset = -1.0f;
    state->transformations->zOffset = -1.0f;

    // Set material
    delete m_meshList->at(m_meshList->size() - 1)->material;
    m_meshList->at(m_meshList->size() - 1)->material = new MaterialEmerald();

    // Apply changes
    m_meshList->at(m_meshList->size() - 1)->mesh->update();
}

void Scene::createDefaultScene()
{
    addGround();
    addPointLight();

    // Adjust light position
    RepeaterState* state = m_meshList->at(m_meshList->size() - 1)->mesh->getState();
    state->transformations->xOffset = 1.0f;
    state->transformations->yOffset = 1.0f;
    state->transformations->zOffset = 1.0f;

    // Apply changes
    m_meshList->at(m_meshList->size() - 1)->mesh->update();
}

void Scene::updateObjectPhysics(int selected)
{
    m_meshList->at(selected)->mesh->updateMeshPhysics(m_physics);
}

void Scene::drawModel(int idx, glm::mat4& projection, glm::mat4& view)
{
    Model* model = m_meshList->at(idx)->model;

    m_lightMeshShader->use();
    m_lightMeshShader->setMat4("projection", projection);
    m_lightMeshShader->setMat4("view", view);

    // Set viewer position
    m_lightMeshShader->setVec3("viewPos", m_camera->Position);

    m_lightHandler->renderAllLightTypes(m_lightMeshShader, true);

    // Set material
    m_lightMeshShader->setBool("materialOverride", false);
    if (m_meshList->at(idx)->material) {
        m_lightMeshShader->setVec3("material.ambient", m_meshList->at(idx)->material->ambient);
        m_lightMeshShader->setVec3("material.diffuse", m_meshList->at(idx)->material->diffuse);
        m_lightMeshShader->setVec3("material.specular", m_meshList->at(idx)->material->specular);
        m_lightMeshShader->setFloat("material.shininess", m_meshList->at(idx)->material->shininess);
    }
    else {
        m_lightMeshShader->setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        m_lightMeshShader->setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        m_lightMeshShader->setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        m_lightMeshShader->setFloat("material.shininess", 64);
    
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
        m_ourShaderInstanced->setInt("selectedMesh", m_meshListHandler->getMeshPointer());
        m_ourShaderInstanced->setInt("selectedInstance", m_meshList->at(idx)->selected);

        m_meshListHandler->highlightChanged();

        m_ourShaderInstanced->setVec3("viewPos", m_camera->Position);

        // Set textures
        m_ourShaderInstanced->setBool("materialOverride", true);
        //m_ourShaderInstanced->setInt("material.diffuse", 0);
        //m_container_texture->use(0);
        //
        //m_ourShaderInstanced->setInt("material.specular", 1);
        //m_container_texture_specular->use(1);

        m_lightHandler->renderAllLightTypes(m_ourShaderInstanced, false);

        // Set material
        if (m_meshList->at(idx)->material) {
            m_ourShaderInstanced->setVec3("materialBase.ambient", m_meshList->at(idx)->material->ambient);
            m_ourShaderInstanced->setVec3("materialBase.diffuse", m_meshList->at(idx)->material->diffuse);
            m_ourShaderInstanced->setVec3("materialBase.specular", m_meshList->at(idx)->material->specular);
            m_ourShaderInstanced->setFloat("materialBase.shininess", m_meshList->at(idx)->material->shininess);
        }
        else {
            std::cout << "Error! Mesh doesn't have material. Using defaults" << std::endl;
            m_ourShaderInstanced->setVec3("materialBase.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
            m_ourShaderInstanced->setVec3("materialBase.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
            m_ourShaderInstanced->setVec3("materialBase.specular", glm::vec3(0.5f, 0.5f, 0.5f));
            m_ourShaderInstanced->setFloat("materialBase.shininess", 32);
            
        }
    }
    else {
        if (m_meshList->at(idx)->lightsEnabled) {
            m_lightMeshShader->use();
            m_lightMeshShader->setMat4("projection", projection);
            m_lightMeshShader->setMat4("view", view);
            m_lightMeshShader->setVec3("viewPos", m_camera->Position);

            // Set textures
            //m_lightMeshShader->setInt("material.diffuse", 0);
            //m_container_texture->use(0);
            //
            //m_lightMeshShader->setInt("material.specular", 1);
            //m_container_texture_specular->use(1);

            m_lightHandler->renderAllLightTypes(m_lightMeshShader, false);

            m_lightMeshShader->setBool("materialOverride", false);
            // Set material
            if (m_meshList->at(idx)->material) {
                
                m_lightMeshShader->setVec3("materialBase.ambient", m_meshList->at(idx)->material->ambient);
                m_lightMeshShader->setVec3("materialBase.diffuse", m_meshList->at(idx)->material->diffuse);
                m_lightMeshShader->setVec3("materialBase.specular", m_meshList->at(idx)->material->specular);
                m_lightMeshShader->setFloat("materialBase.shininess", m_meshList->at(idx)->material->shininess);
            }
            else {
                std::cout << "Error! Mesh doesn't have material. Using defaults" << std::endl;
                m_lightMeshShader->setVec3("materialBase.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
                m_lightMeshShader->setVec3("materialBase.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
                m_lightMeshShader->setVec3("materialBase.specular", glm::vec3(0.5f, 0.5f, 0.5f));
                m_lightMeshShader->setFloat("materialBase.shininess", 32);
            }
        }

        // Set light shader
        if (m_meshList->at(idx)->type == MeshType::PointLightType ||
            m_meshList->at(idx)->type == MeshType::DirectionalLightType ||
            m_meshList->at(idx)->type == MeshType::SpotLightType) {
            m_lightShader->use();
            m_lightShader->setMat4("projection", projection);
            m_lightShader->setMat4("view", view);
        }
    }

    m_meshList->at(idx)->mesh->draw(m_physics, m_mousePicker, m_camera);
}

void Scene::renderScene()
{
    // Get the current projection matrix
    glm::mat4 projection = glm::perspective(
        glm::radians(m_camera->Zoom),
        (float)m_screenSettings->width / (float)m_screenSettings->height, 0.1f, 100.0f
    );

    // Get the current view matrix
    glm::mat4 view = m_camera->GetViewMatrix();

    // Update mouse-over
    m_mousePicker->update(projection, view);

    // Draw terrain
    m_lightMeshShader->use();
    m_lightMeshShader->setMat4("projection", projection);
    m_lightMeshShader->setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
    //m_container_texture->use(0);
    m_lightMeshShader->setMat4("model", model);
    m_terrain->render();

    //model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(m_terrain2->getX(), 0.0, m_terrain2->getZ()));
    //model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
    //m_terrainShader->setMat4("aInstanceMatrix", model);
    //m_terrain2->render();

    // Draw all objects
    for (int i = 0; i < m_meshList->size(); i++) {
        RepeaterState* state;
        if (m_meshList->at(i)->type == MeshType::ModelType)
            drawModel(i, projection, view);
        else
            draw(i, projection, view);
    }

    // Update mouse-over
    m_mousePicker->update(projection, view);
    //if (m_mousePicker->testRayIntersection(projection)) {
    //    std::cout << "Intersect ptr: " << std::endl;
    //}

    // Update physics
    m_physics->update(projection, view);
    //glm::vec3 ray_origin = m_mousePicker->getRayOrigin();
    //glm::vec3 ray_direction = m_mousePicker->getRayDirection();
    //glm::vec3 ray_end = ray_origin + ray_direction * 1000.0f;
    //m_physics->hasHit(ray_origin, ray_end);
}

void Scene::deleteObject(int idx)
{
    // Delete pointlights
    if (m_meshList->at(idx)->type == MeshType::PointLightType) {
        m_lightHandler->deletePointLight(m_meshList->at(idx)->name);
    }

    // Delete directional lights
    if (m_meshList->at(idx)->type == MeshType::DirectionalLightType) {
        m_lightHandler->deleteDirectionalLight(m_meshList->at(idx)->name);
    }

    // Delete spot lights
    if (m_meshList->at(idx)->type == MeshType::SpotLightType) {
        m_lightHandler->deleteSpotLight(m_meshList->at(idx)->name);
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

    // Clear material
    delete m_meshList->at(idx)->material;

    // Clear model
    delete m_meshList->at(idx)->model;

    // Clear model object
    delete m_meshList->at(idx);
    
    // Clear the object list
    m_meshList->erase(m_meshList->begin() + idx);
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