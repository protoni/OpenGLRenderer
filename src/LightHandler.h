#ifndef LIGHT_HANDLER_H
#define LIGHT_HANDLER_H

#include "MeshObject.h"
#include "Shader.h"
#include <vector>

class LightHandler
{
public:
    LightHandler(
        std::vector<MeshObject*>& pointLights,
        std::vector<MeshObject*>& directionalLights,
        std::vector<MeshObject*>& spotLights
    );
    ~LightHandler();

    // Render all the lights
    void renderAllLightTypes(Shader* shader, bool invertPos);

    // Delete all the lights
    void deleteAllLightTypes();

    // Delete specific lights with a given name
    void deletePointLight(std::string& name);
    void deleteDirectionalLight(std::string& name);
    void deleteSpotLight(std::string& name);

    // Add new pointlight object to the list
    void addPointLight(MeshObject* object);

    // Add new directional light object to the list
    void addDirectionalLight(MeshObject* object);

    // Add new spotlight object to the list
    void addSpotLight(MeshObject* object);

private:

    // Set shader uniforms for different type of lights
    void renderPointLight(int idx, Shader* shader, bool invertPos = false);
    void renderDirectionalLight(int idx, Shader* shader, bool invertPos = false);
    void renderSpotLight(int idx, Shader* shader, bool invertPos = false);

    // Store all the different type lights in vectors
    std::vector<MeshObject*> m_pointLights;
    std::vector<MeshObject*> m_directionalLights;
    std::vector<MeshObject*> m_spotLights;
};

#endif // LIGHT_HANDLER_H