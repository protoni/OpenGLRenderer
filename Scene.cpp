#include "Scene.h"

#include "stb_image.h"

float multiply = 1.0f;

float plane_vertices_default[] = {
    // Location         // Texture coords
    -0.5f, 0.0f,  0.5f,  0.0f, 0.0f, // far left
     0.5f, 0.0f,  0.5f,  0.0f, 0.0f, // far rigt
     0.5f, 0.0f, -0.5f,  0.0f, 0.0f, // near right
    -0.5f, 0.0f, -0.5f,  0.0f, 0.0f, // near left
};

float plane_vertices[] = {
    // Location         // Texture coords
    -0.5f, 0.0f,  0.5f,  0.0f, 0.0f, // far left
     0.5f, 0.0f,  0.5f,  0.0f, 0.0f, // far rigt
     0.5f, 0.0f, -0.5f,  0.0f, 0.0f, // near right
    -0.5f, 0.0f, -0.5f,  0.0f, 0.0f, // near left
};

unsigned int plane_indices[] = {
    0, 1, 2,
    2, 3, 0
};

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
    m_ourShader(NULL), m_texture1(0), m_texture2(0), m_VAO(0), m_EBO(0), m_hexagon_mesh(NULL),
    m_smiley_texture(NULL), m_multiplier(1.0)
{
    m_ourShader = new Shader("./shader.vs", "./shader.fs");

    createHexagon();

    // Load texture
    m_smiley_texture = new Texture(m_ourShader, "awesomeface.png");
}

Scene::~Scene()
{
}

void Scene::createHexagon()
{
    m_hexagon_mesh = new Mesh(m_ourShader, plane_vertices, plane_indices,
        sizeof(plane_vertices) / sizeof(plane_vertices[0]),
        sizeof(plane_indices)  / sizeof(plane_indices[0]));
    
}

void Scene::resizeHexagon(float multiplier)
{
    // Adjust multiplier
    m_multiplier = multiplier;
    std::cout << "multiplier: " << m_multiplier << std::endl;

    // Resize the plane
    for (int i = 0; i < sizeof(plane_vertices) / sizeof(plane_vertices[0]); i++) {
        plane_vertices[i] = plane_vertices_default[i] * m_multiplier;
    }

    // Clean old one
    if (m_hexagon_mesh)
        delete(m_hexagon_mesh);

    // Create new hexagon
    createHexagon();
}

float Scene::getMultiplier()
{
    return m_multiplier;
}

void Scene::renderScene()
{
    if (!m_ourShader)
        return;

    m_ourShader->use();
    m_smiley_texture->use();

    if (m_hexagon_mesh) {
        m_hexagon_mesh->render(m_faceCounter);
    }

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