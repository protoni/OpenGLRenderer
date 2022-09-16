#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{

public:
    Texture(const char* path);
    ~Texture();

    bool load(bool useRGB = false);
    void use(int offset);
    unsigned int getTexture();

private:
    const char* m_path;

    unsigned int m_texture;
};

#endif // TEXTURE_H