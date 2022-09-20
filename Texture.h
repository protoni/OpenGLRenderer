#ifndef TEXTURE_H
#define TEXTURE_H

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

private:
    /* Path of the texture file */
    const char* m_path;

    /* Does the texture image use RGBA channel */
    bool m_useRGBA;

    unsigned int m_texture;
};

#endif // TEXTURE_H