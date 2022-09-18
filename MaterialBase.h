#ifndef MATERIAL_BASE_H
#define MATERIAL_BASE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>


struct MaterialBase
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    std::string name;

    MaterialBase(
        glm::vec3 inAmbient,
        glm::vec3 inDiffuse,
        glm::vec3 inSpecular,
        float inShininess,
        std::string name
    )
        : ambient(inAmbient)
        , diffuse(inDiffuse)
        , specular(inSpecular)
        , shininess(inShininess)
        , name(name) {}
};

/* Default */
struct MaterialDefault : MaterialBase
{
    MaterialDefault() : MaterialBase(
        glm::vec3(1.0f, 0.5f, 0.31f),
        glm::vec3(1.0f, 0.5f, 0.31f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        32.0f,
        "Default") {}
};

/* Emerald */
struct MaterialEmerald : MaterialBase
{
    MaterialEmerald() : MaterialBase(
        glm::vec3(0.0215f, 0.1745f, 0.0215f),
        glm::vec3(0.07568f, 0.61424f, 0.07568f),
        glm::vec3(0.633f, 0.727811f, 0.633f),
        0.6,
        "Emerald") {}
};

/* Silver */
struct MaterialSilver : MaterialBase
{
    MaterialSilver() : MaterialBase(
        glm::vec3(0.19225f, 0.19225f, 0.19225f),
        glm::vec3(0.50754f, 0.50754f, 0.50754f),
        glm::vec3(0.508273f, 0.508273f, 0.508273f),
        0.4,
        "Silver") {}
};

/* Green Rubber */
struct MaterialGreenRubber : MaterialBase
{
    MaterialGreenRubber() : MaterialBase(
        glm::vec3(0.0f, 0.05f, 0.0f),
        glm::vec3(0.4f, 0.5f, 0.4f),
        glm::vec3(0.04f, 0.7f, 0.04f),
        0.078125,
        "GreenRubber") {}
};

/* Yellow Rubber */
struct MaterialYellowRubber : MaterialBase
{
    MaterialYellowRubber() : MaterialBase(
        glm::vec3(0.05f, 0.05f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.4f),
        glm::vec3(0.7f, 0.7f, 0.04f),
        0.078125,
        "YellowRubber") {}
};

struct MaterialCollection
{
    MaterialDefault materialDefault;
    MaterialEmerald materialEmerald;
    MaterialSilver materialSilver;
    MaterialGreenRubber materialGreenRubber;
    MaterialYellowRubber materialYellowRubber;

    std::vector<MaterialBase> materials;
    MaterialCollection()
        : materials({
            materialDefault,
            materialEmerald,
            materialSilver,
            materialGreenRubber,
            materialYellowRubber
          }) {}
};

class Material
{
public:
    static const char** getMaterialNames()
    {
        
        MaterialCollection collection;

        std::cout << "collection.materialDefault.name.c_str(): " << collection.materialDefault.name.c_str() << std::endl;

        //collection.materialDefault.name.c_str(),
            //collection.materialEmerald.name.c_str(),
            //collection.materialSilver.name.c_str(),
            //collection.materialGreenRubber.name.c_str(),
            //collection.materialYellowRubber.name.c_str()

        const char* MaterialTypeNames[] = {
            "Custom",
            "Default",
            "Emerald"
            
        };

        return MaterialTypeNames;
    }
};

#endif // MATERIAL_BASE_H