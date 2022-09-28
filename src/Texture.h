#ifndef TEXTURE_H
#define TEXTURE_H

#include "stb_image.h"

class Texture
{

public:
    Texture(const char* path, bool useRGBA = false);
    ~Texture();

    bool load();
    void use(int offset);
    unsigned int getTexture();

    /* Deactivate texture */
    static void deactivate();

    // Get image data
    unsigned char* getData() { return m_data; }

    // Get image properties
    int getWidth() { return m_width; }
    int getHeight() { return m_height; }
    int getNrChannels() { return m_nrChannels; }

    // Get image's RGB value
    float getRGB(int x, int y);

private:
    /* Path of the texture file */
    const char* m_path;

    /* Does the texture image use RGBA channel */
    bool m_useRGBA;

    unsigned int m_texture;

    // Store image data here
    stbi_uc* m_data;

    // Store image properties here
    int m_width = 0;
    int m_height = 0;
    int m_nrChannels = 0;
};

#endif // TEXTURE_H