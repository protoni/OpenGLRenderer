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
    m_ourShader(NULL), m_texture1(0), m_texture2(0), m_VAO(0), m_EBO(0), m_hexagon_mesh(NULL)
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
    
    // Generate and load textures
    glGenTextures(1, &m_texture1);
    glGenTextures(1, &m_texture2);
    glBindTexture(GL_TEXTURE_2D, m_texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;

    // Flip y-axis
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, m_texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    m_ourShader->use();
    glUniform1i(glGetUniformLocation(m_texture1, "texture1"), 0);
    m_ourShader->setInt("texture2", 1);

   m_hexagon_mesh->setTexture1(m_texture1);
   m_hexagon_mesh->setTexture2(m_texture2);
}

void Scene::renderScene()
{
    m_hexagon_mesh->render(m_faceCounter);

    if (!m_ourShader)
        return;

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