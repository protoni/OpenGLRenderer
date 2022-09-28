#include "Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "DebugMacros.h"

#include <iostream>

Texture::Texture(const char* path, bool useRGBA) : m_path(path), m_texture(0), m_useRGBA(useRGBA)
{
    load();
}

Texture::~Texture()
{
    stbi_image_free(m_data);
}

bool Texture::load()
{
    bool ret = false;
    // Generate and load texture
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //int width, height, nrChannels;

    // Flip y-axis
    stbi_set_flip_vertically_on_load(true);

    //unsigned char* data;

    int desiredChannels = 0;
    if (m_useRGBA)
        desiredChannels = 4;

    m_data = stbi_load(m_path, &m_width, &m_height, &m_nrChannels, desiredChannels);

    if (m_data) {
        if(m_useRGBA)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);

        glGenerateMipmap(GL_TEXTURE_2D);
        //stbi_image_free(data);
        ret = true;
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    return ret;
}

void Texture::use(int offset)
{
    glActiveTexture(GL_TEXTURE0 + offset);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::deactivate()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

float Texture::getRGB(int x, int y)
{
    const stbi_uc* p = m_data + (4 * (y + m_width * x));

    int rgb = (p[0] << 16) | (p[1] << 8) | p[2];

    return (float)rgb;
}

unsigned int Texture::getTexture()
{
    return m_texture;
}


