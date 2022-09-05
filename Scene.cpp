#include "Scene.h"

#include "stb_image.h"

float multiply = 1.0f;

#include "DebugMacros.h"

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
    m_ourShaderInstanced(NULL), m_instanced_cube(false), m_meshPointer(0)
{
    // Create and build shaders
    m_ourShaderInstanced = new Shader("./shaderInstanced.vs", "./shader.fs");
    m_ourShader = new Shader("./shader.vs", "./shader.fs");
    
    // Load texture
    m_smiley_texture = new Texture("awesomeface.png");

    // Create mesh vector
    m_meshList = new std::vector<MeshObject*>;

}

Scene::~Scene()
{
    if (m_meshList) {
        delete m_meshList;
        m_meshList = NULL;
    }

    if (m_smiley_texture) {
        delete m_smiley_texture;
        m_smiley_texture = NULL;
    }

    if (m_ourShaderInstanced) {
        delete m_ourShaderInstanced;
        m_ourShaderInstanced = NULL;
    }

    if (m_ourShader) {
        delete m_ourShader;
        m_ourShader = NULL;
    }
}

void Scene::updateMeshShader(bool instanced, int idx)
{
    if (instanced) {
        m_meshList->at(idx)->mesh->setShader(m_ourShaderInstanced);
    }
    else {
        m_meshList->at(idx)->mesh->setShader(m_ourShader);
    }

    m_meshList->at(idx)->mesh->setInstanced(instanced);
}

void Scene::addCube()
{
    Cube* cube = new Cube(m_ourShader, false);
    MeshObject* object = new MeshObject();
    object->mesh = cube;
    object->name = std::string("Cube_") + std::to_string(m_meshList->size());
    m_meshList->push_back(object);
}

void Scene::addPlane()
{
    Plane* plane = new Plane(m_ourShader, false);
    MeshObject* object = new MeshObject();
    object->mesh = plane;
    object->name = std::string("Plane_") + std::to_string(m_meshList->size());
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

void Scene::updateMeshPointer(int direction)
{
    RepeaterState* state = m_meshList->at(getSelectedMeshIndex())->mesh->getState();

    // Figure out which row are we on
    int rowPosition;
    if (m_meshPointer > 0) {
        rowPosition = (m_meshPointer / state->columnCount);
    }
    else {
        rowPosition = 0;
    }

    std::cout << "rowPosition: " << rowPosition << std::endl;

    if (direction == MeshInstanceDirections::Left) {
        if(m_meshPointer - (rowPosition * state->columnCount) > 0)
            m_meshPointer -= 1;
    }

    else if (direction == MeshInstanceDirections::Right) {
        if(m_meshPointer - (rowPosition * state->columnCount) < state->rowCount - 1)
            m_meshPointer += 1;
    }

    else if (direction == MeshInstanceDirections::Down) {
        if(rowPosition < state->rowCount - 1)
            m_meshPointer += state->columnCount;
    }

    else if (direction == MeshInstanceDirections::Up) {
        if (rowPosition > 0)
            m_meshPointer -= state->columnCount;
    }

    std::cout << "mesh pointer: " << m_meshPointer << std::endl;
}

void Scene::resetMeshPointer()
{
    m_meshPointer = 0;
}

std::vector<MeshObject*>* Scene::getMeshList()
{
    return m_meshList;
}

int Scene::getSelectedMeshIndex()
{
    for (int i = 0; i < m_meshList->size(); i++) {
        if (m_meshList->at(i)->selected)
            return i;
    }

    return -1;
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
        count += m_meshList->at(i)->mesh->getObjCount() * m_meshList->at(i)->mesh->getIndexCount();
        //if (m_meshList->at(i)->name.find("Cube") != std::string::npos)
        //    count += m_meshList->at(i)->mesh->getObjCount() * ( 6 * 6); // 6 plane indices * 6 faces of a cube
        //else
        //    count += m_meshList->at(i)->mesh->getObjCount() * 6; // 6 indices of a plane
    }

    return count;
}

int Scene::getObjectCount()
{
    int count = 0;

    for (int i = 0; i < m_meshList->size(); i++) {
        count += m_meshList->at(i)->mesh->getObjCount();
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
            m_ourShaderInstanced->setInt("selectedMesh", m_meshPointer);
            m_ourShaderInstanced->setInt("selectedInstance", m_meshList->at(i)->selected);
            //if (m_meshList->at(i)->selected) {
            //    m_ourShaderInstanced->setVec4("selectColor", glm::vec4(0.2, 0.0, 0.0, 0.5));
            //}
            //else
            //    m_ourShaderInstanced->setVec4("selectColor", glm::vec4(0.0, 0.0, 0.0, 0.0));
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

void Scene::deleteObject(int idx)
{
    delete m_meshList->at(idx)->mesh;
    delete m_meshList->at(idx);

    m_meshList->erase(m_meshList->begin() + idx);
}

void Scene::clean()
{
    std::cout << "Cleaning: " << m_meshList->size() << " objects!" << std::endl;

    for (int i = m_meshList->size() -1; i >= 0; i--) {
        deleteObject(i);
    }

    m_meshList->clear();
}