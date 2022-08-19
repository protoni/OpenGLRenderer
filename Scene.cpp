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
    m_ourShader(NULL), m_texture1(0), m_texture2(0), m_VAO(0), m_EBO(0)
{
    m_ourShader = new Shader("./shader.vs", "./shader.fs");

    createHexagon();
}

Scene::~Scene()
{
}



void Scene::createHexagon()
{
    unsigned int VBO;
    glGenVertexArrays(1, &m_VAO);

    // Create Buffers
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &m_EBO);

    // Use vertex array
    glBindVertexArray(m_VAO);

    // hexagon
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(hexagon_vertices), hexagon_vertices, GL_STATIC_DRAW);

    // hexagon
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hexagon_indices), hexagon_indices, GL_STATIC_DRAW);

    // Link vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

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
}

void Scene::renderHexagon()
{
    if (!m_ourShader || !m_texture1 || !m_texture2 || !m_VAO || !m_EBO)
        return;

    m_ourShader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2);

    glBindVertexArray(m_VAO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
    m_ourShader->setMat4("model", model);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glDrawElements(GL_TRIANGLES, m_faceCounter, GL_UNSIGNED_INT, 0);
    
}

void Scene::renderScene()
{
    renderHexagon();

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