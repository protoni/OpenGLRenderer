#ifndef MESH_OBJECT_H
#define MESH_OBJECT_H

#include <glm/glm.hpp>
#include <string>

#include "Repeater.h"
#include "Model.h"
#include "MaterialBase.h"

enum MeshType
{
    CubeType,
    CustomType,
    PlaneType,
    SphereType,
    TriangleType,
    PointLightType,
    DirectionalLightType,
    ReflectCubeType,
    SpotLightType,
    ModelType,
    UnknownType
};

struct MeshLights
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    float constant;
    float linear;
    float quadratic;

    MeshLights()
        : ambient(0.2f, 0.2f, 0.2f)
        , diffuse(0.5f, 0.5f, 0.5f)
        , specular(1.0f, 1.0f, 1.0f)
        , shininess(32.0f)
        , constant(1.0f)
        , linear(0.09f)
        , quadratic(32.0f) {}
};

struct MeshObject
{
    Repeater* mesh;
    Model* model;
    std::string name;
    bool selected;

    // Mesh lighting settings
    MeshLights* light;

    // Mesh material settings
    MaterialBase* material;

    // What type of mesh is this
    int type;

    // Is the object reflecting light
    bool lightsEnabled;

    MeshObject()
        : mesh(nullptr)
        , model(nullptr)
        , light(nullptr)
        , material(nullptr)
        , name("")
        , selected(false)
        , lightsEnabled(false)
        , type(-1) {}
};

#endif // MESH_OBJECT_H