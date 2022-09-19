#include "Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "DebugMacros.h"

Texture::Texture(const char* path, bool useRGBA) : m_path(path), m_texture(0), m_useRGBA(useRGBA)
{
    load();
}

Texture::~Texture()
{

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

    int width, height, nrChannels;

    // Flip y-axis
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data;

    int desiredChannels = 0;
    if (m_useRGBA)
        desiredChannels = 4;

    data = stbi_load(m_path, &width, &height, &nrChannels, desiredChannels);

    if (data) {
        if(m_useRGBA)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
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

unsigned int Texture::getTexture()
{
    return m_texture;
}
