#ifndef MATERIAL_BASE_H
#define MATERIAL_BASE_H

#include <glad_opengl4_3/glad.h>
#include <GLFW/glfw3.h>

namespace Materials
{

struct MaterialBase
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    MaterialBase(glm::vec3 inAmbient, glm::vec3 inDiffuse, glm::vec3 inSpecular, float inShininess)
        : ambient(inAmbient)
        , diffuse(inDiffuse)
        , specular(inSpecular)
        , shininess(inShininess) {}
};

/* Emerald */
MaterialBase materialTypeEmerald(
    glm::vec3(0.0215f, 0.1745f, 0.0215f),
    glm::vec3(0.07568f, 0.61424f, 0.07568f),
    glm::vec3(0.633f, 0.727811f, 0.633f),
    0.6
);

/* Silver */
MaterialBase materialTypeSilver(
    glm::vec3(0.19225f, 0.19225f, 0.19225f),
    glm::vec3(0.50754f, 0.50754f, 0.50754f),
    glm::vec3(0.508273f, 0.508273f, 0.508273f),
    0.4
);

/* Green rubber */
MaterialBase materialTypeGreenRubber(
    glm::vec3(0.0f, 0.05f, 0.0f),
    glm::vec3(0.4f, 0.5f, 0.4f),
    glm::vec3(0.04f, 0.7f, 0.04f),
    0.078125
);

/* Yellow rubber */
MaterialBase materialTypeYellowRubber(
    glm::vec3(0.05f, 0.05f, 0.0f),
    glm::vec3(0.5f, 0.5f, 0.4f),
    glm::vec3(0.7f, 0.7f, 0.04f),
    0.078125
);

}
#endif // MATERIAL_BASE_H