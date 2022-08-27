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

Scene::Scene(Camera *camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT) :
    m_camera(camera), m_screenWidth(SCR_WIDTH), m_screenHeight(SCR_HEIGHT), m_faceCounter(3),
    m_ourShader(NULL), m_texture1(0), m_texture2(0), m_VAO(0), m_EBO(0), m_plane_mesh(NULL),
    m_smiley_texture(NULL), m_columns(1), m_meshList(NULL), m_scale(1.0), m_rows(1), m_instanced(false),
    m_ourShaderInstanced(NULL)
{
    m_ourShaderInstanced = new Shader("./shaderInstanced.vs", "./shader.fs");
    m_ourShader = new Shader("./shader.vs", "./shader.fs");
    
    createPlane();

    // Load texture
    m_smiley_texture = new Texture("awesomeface.png");

    // Create mesh list
    std::vector<Mesh*> m_meshList;
}

Scene::~Scene()
{
    if (m_plane_mesh)
        delete(m_plane_mesh);
}

void Scene::createPlane()
{
    if (m_instanced) {
        std::cout << "using instanced plane shader!" << std::endl;
        m_plane_mesh = new Plane(m_ourShaderInstanced, m_instanced);
    }
    else {
        std::cout << "using non instanced plane shader!" << std::endl;
        m_plane_mesh = new Plane(m_ourShader, m_instanced);
    }
}

void Scene::changePlaneInstanced(bool instanced)
{
    m_instanced = instanced;

    if(m_plane_mesh)
        delete(m_plane_mesh);

    createPlane();
}

bool Scene::getPlaneInstanceMode()
{
    return m_instanced;
}

void Scene::updatePlane(int rows, int columns, float scale)
{
    m_plane_mesh->update(rows, columns, scale);
    std::cout << "Triangle count: " << rows * columns * 6 << std::endl;
}

void Scene::renderScene()
{
    if (!m_ourShader || !m_ourShaderInstanced) {
        std::cout << "Scene::renderScene::shader error!" << std::endl;
        return;
    }

    m_smiley_texture->use();

    glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), (float)m_screenWidth / (float)m_screenHeight, 0.1f, 100.0f);
    glm::mat4 view = m_camera->GetViewMatrix();

    if (m_instanced) {
        m_ourShaderInstanced->use();
        m_plane_mesh->drawInstanced();
        m_ourShaderInstanced->setMat4("projection", projection);
        m_ourShaderInstanced->setMat4("view", view);
    }
    else {
        m_ourShader->use();
        m_plane_mesh->drawNonInstanced();
        m_ourShader->setMat4("projection", projection);
        m_ourShader->setMat4("view", view);
    }
}

void Scene::update()
{
    m_faceCounter += 3;
    if (m_faceCounter >= 21) {
        m_faceCounter = 3;
    }
}