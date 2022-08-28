#include "Scene.h"

#include "stb_image.h"

float multiply = 1.0f;


float hexagon_vertices[] = {
    -0.5f,  1.0f, 0.0f,  0.0f, 0.0f,  // top left
     0.5f,  1.0f, 0.0f,  1.0f, 0.0f,  // top right
     1.0f,  0.0f, 0.0f,  1.0f, 1.0f,  // center right
     0.5f, -1.0f, 0.0f,  1.0f, 1.0f,  // bottom right
    -0.5f, -1.0f, 0.0f,  0.0f, 1.0f,  // bottom left
    -1.0f,  0.0f, 0.0f,  0.0f, 0.0f,  // center left
     0.0f,  0.0f, 0.0f,  1.0f, 0.0f   // center
};

unsigned int hexagon_indices[] = {
    6, 0, 1, // top
    6, 1, 2, // top right
    6, 2, 3, // bottom right
    6, 3, 4, // bottom
    6, 4, 5, // bottom left
    6, 5, 0  // top left
};

Scene::Scene(Camera *camera, ScreenSettings* screenSettings) :
    m_camera(camera), m_screenSettings(screenSettings), m_faceCounter(3),
    m_ourShader(NULL), m_texture1(0), m_texture2(0), m_VAO(0), m_EBO(0),
    m_smiley_texture(NULL), m_columns(1), m_meshList(), m_scale(1.0), m_rows(1), m_instanced(false),
    m_ourShaderInstanced(NULL), m_instanced_cube(false)
{
    m_ourShaderInstanced = new Shader("./shaderInstanced.vs", "./shader.fs");
    m_ourShader = new Shader("./shader.vs", "./shader.fs");
    
    //createPlane();
    //createCube();

    // Load texture
    m_smiley_texture = new Texture("awesomeface.png");

    // Create mesh vector
    m_meshList = new std::vector<MeshObject*>;
}

Scene::~Scene()
{
    for (int i = 0; i < m_meshList->size(); i++) {
        delete(m_meshList->at(i)->mesh);
    }
}

Plane* Scene::createPlane(bool instanced)
{
    Plane* plane;

    if (instanced) {
        std::cout << "using instanced plane shader!" << std::endl;
        plane = new Plane(m_ourShaderInstanced, instanced);
    }
    else {
        std::cout << "using non instanced plane shader!" << std::endl;
        plane = new Plane(m_ourShader, instanced);
    }

    return plane;
}

Cube* Scene::createCube(bool instanced)
{
    Cube* cube;

    if (instanced) {
        std::cout << "using instanced cube shader!" << std::endl;
        cube = new Cube(m_ourShaderInstanced, instanced);
    }
    else {
        std::cout << "using non instanced cube shader!" << std::endl;
        cube = new Cube(m_ourShader, instanced);
    }

    return cube;
}

void Scene::updatePlaneInstanced(bool instanced, int idx)
{
    delete(m_meshList->at(idx)->mesh);
    m_meshList->at(idx)->mesh = createPlane(instanced);
}

void Scene::updateCubeInstanced(bool instanced, int idx)
{
    delete(m_meshList->at(idx)->mesh);
    m_meshList->at(idx)->mesh = createCube(instanced);
}

void Scene::addCube()
{
    MeshObject* object = new MeshObject();
    object->mesh = createCube(false);
    object->name = std::string("Cube_") + std::to_string(m_meshList->size());

    m_meshList->push_back(object);
}

void Scene::addPlane()
{
    MeshObject* object = new MeshObject();
    object->mesh = createPlane(false);
    object->name = std::string("Plane_") + std::to_string(m_meshList->size());

    m_meshList->push_back(object);
}

std::vector<MeshObject*>* Scene::getMeshList()
{
    return m_meshList;
}

bool Scene::updateObjectMesh(int idx)
{
    if (!m_meshList->size() > idx) {
        return false;
    }
    
    m_meshList->at(idx)->mesh->update();

    std::cout << "Triangle count: " << getTriangleCount() << std::endl;

    return true;
}

int Scene::getTriangleCount()
{
    int count = 0;
    
    for (int i = 0; i < m_meshList->size(); i++) {
        if (m_meshList->at(i)->name.find("Cube") != std::string::npos)
            count += m_meshList->at(i)->mesh->getObjCount() * ( 6 * 6); // 6 plane indices * 6 faces of a cube
        else
            count += m_meshList->at(i)->mesh->getObjCount() * 6; // 6 indices of a plane
    }

    return count;
}

void Scene::renderScene()
{
    if (!m_ourShader || !m_ourShaderInstanced) {
        std::cout << "Scene::renderScene::shader error!" << std::endl;
        return;
    }

    m_smiley_texture->use();

    glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), (float)m_screenSettings->width / (float)m_screenSettings->height, 0.1f, 100.0f);
    glm::mat4 view = m_camera->GetViewMatrix();

    for (int i = 0; i < m_meshList->size(); i++) {
        RepeaterState* state = m_meshList->at(i)->mesh->getState();
        if (state->instanced) {
            m_ourShaderInstanced->use();
            m_ourShaderInstanced->setMat4("projection", projection);
            m_ourShaderInstanced->setMat4("view", view);

            if (m_meshList->at(i)->selected) {
                m_ourShaderInstanced->setVec4("selectColor", glm::vec4(0.2, 0.0, 0.0, 0.5));
            }
            else
                m_ourShaderInstanced->setVec4("selectColor", glm::vec4(0.0, 0.0, 0.0, 0.0));
        }
        else {
            m_ourShader->use();
            m_ourShader->setMat4("projection", projection);
            m_ourShader->setMat4("view", view);

            if (m_meshList->at(i)->selected)
                m_ourShader->setVec4("selectColor", glm::vec4(0.2, 0.0, 0.0, 0.5));
            else
                m_ourShader->setVec4("selectColor", glm::vec4(0.0, 0.0, 0.0, 0.0));
        }

        if (m_meshList->at(i)->name.find("Cube") != std::string::npos)
            m_meshList->at(i)->mesh->draw(6);
        else
            m_meshList->at(i)->mesh->draw();

        

    }
}

void Scene::update()
{
    m_faceCounter += 3;
    if (m_faceCounter >= 21) {
        m_faceCounter = 3;
    }
}