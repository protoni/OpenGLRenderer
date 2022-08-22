#include "Scene.h"

#include "stb_image.h"

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
    20, 0, 1, // top
    20, 1, 2, // top right
    20, 2, 3, // bottom right
    20, 3, 4, // bottom
    20, 4, 5, // bottom left
    20, 5, 0  // top left
};

Scene::Scene(Camera *camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT) :
    m_camera(camera), m_screenWidth(SCR_WIDTH), m_screenHeight(SCR_HEIGHT), m_faceCounter(3),
    m_ourShader(NULL), m_texture1(0), m_texture2(0), m_VAO(0), m_EBO(0), m_hexagon_mesh(NULL),
    m_smiley_texture(NULL)
{
    m_ourShader = new Shader("./shader.vs", "./shader.fs");

    createHexagon();
}

Scene::~Scene()
{
}

void Scene::createHexagon()
{

    m_hexagon_mesh = new Mesh(m_ourShader, hexagon_vertices, hexagon_indices,
        sizeof(hexagon_vertices) / sizeof(hexagon_vertices[0]),
        sizeof(hexagon_indices)  / sizeof(hexagon_indices[0]));
    
    // Load texture
    m_smiley_texture = new Texture(m_ourShader, "awesomeface.png");

}

void Scene::renderScene()
{
    if (!m_ourShader)
        return;

    m_ourShader->use();
    m_smiley_texture->use();
    m_hexagon_mesh->render(m_faceCounter);

    glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), (float)m_screenWidth / (float)m_screenHeight, 0.1f, 100.0f);
    m_ourShader->setMat4("projection", projection);

    glm::mat4 view = m_camera->GetViewMatrix();
    m_ourShader->setMat4("view", view);
}

void Scene::update()
{
    m_faceCounter += 3;
    if (m_faceCounter >= 21) {
        m_faceCounter = 3;
    }
}