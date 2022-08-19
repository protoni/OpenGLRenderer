#ifndef TEXTURE_H
#define TEXTURE_H

#include "Shader.h"

class Texture
{

public:
    Texture(Shader *shader, const char* path);
    ~Texture();

    void load();
    void use();
    unsigned int getTexture();

private:
    Shader* m_shader;

    const char* m_path;

    unsigned int m_texture;
};

#endif // TEXTURE_H