#include "LightHandler.h"
#include "RepeaterState.h"

#include <glm/glm.hpp>

LightHandler::LightHandler(
    std::vector<MeshObject*>& pointLights,
    std::vector<MeshObject*>& directionalLights,
    std::vector<MeshObject*>& spotLights
) : m_pointLights(pointLights), m_directionalLights(directionalLights), m_spotLights(spotLights)
{
}

LightHandler::~LightHandler()
{
}

void LightHandler::renderDirectionalLight(int idx, Shader* shader, bool invertPos)
{
    // Return if max point light count
    if (idx >= 40)
        return;

    // Get light's mesh state
    RepeaterState* state = m_directionalLights.at(idx)->mesh->getState();

    // Get light position
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    if (state->modified->size() > 0) {
        pos.x = state->modified->at(0)->transformations->xPos;
        pos.y = state->modified->at(0)->transformations->yPos;
        pos.z = state->modified->at(0)->transformations->zPos;
    }

    // Create uniform array location name
    std::string prefix = "dirLights[";
    std::string idxStr = std::to_string(idx);


    // Set light position
    std::string name = prefix + idxStr + "].direction";
    // m_ourShaderInstanced->setVec3(name, pos);

     // Set light direction
     //name = prefix + idxStr + "].direction";
    if(invertPos)
        shader->setVec3(name, -pos);
    else
        shader->setVec3(name, pos);

    // Set light values
    name = prefix + idxStr + "].ambient";
    shader->setVec3(name, glm::vec3(0.05f, 0.05f, 0.05f));

    name = prefix + idxStr + "].diffuse";
    shader->setVec3(name, glm::vec3(0.4f, 0.4f, 0.4f));

    name = prefix + idxStr + "].specular";
    shader->setVec3(name, glm::vec3(0.5f, 0.5f, 0.5f));

}

void LightHandler::renderSpotLight(int idx, Shader* shader, bool invertPos)
{
    // Return if max point light count
    if (idx >= 40)
        return;

    // Get light's mesh state
    RepeaterState* state = m_spotLights.at(idx)->mesh->getState();

    // Get light position
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    if (state->modified->size() > 0) {
        pos.x = state->modified->at(0)->transformations->xPos;
        pos.y = state->modified->at(0)->transformations->yPos;
        pos.z = state->modified->at(0)->transformations->zPos;
    }

    // Create uniform array location name
    std::string prefix = "spotLights[";
    std::string idxStr = std::to_string(idx);


    // Set light position
    std::string name = prefix + idxStr + "].position";
    //if (invertPos) // Invert light position when rendering a model
    //    shader->setVec3(name, -pos);
    //else
        shader->setVec3(name, pos);

    // Set light direction
    name = prefix + idxStr + "].direction";
    //if(invertPos)
    //    shader->setVec3(name, glm::vec3(-1.0f, 0.0f, 0.0f));
    //else
        shader->setVec3(name, glm::vec3(1.0f, 0.0f, 0.0f));


    // Set light values
    name = prefix + idxStr + "].ambient";
    shader->setVec3(name, glm::vec3(0.0f, 0.0f, 0.0f));

    name = prefix + idxStr + "].diffuse";
    shader->setVec3(name, glm::vec3(1.0f, 1.0f, 1.0f));

    name = prefix + idxStr + "].specular";
    shader->setVec3(name, glm::vec3(1.0f, 1.0f, 1.0f));


    // Set fade-out values
    name = prefix + idxStr + "].constant";
    shader->setFloat(name, 1.0f);

    name = prefix + idxStr + "].linear";
    shader->setFloat(name, 0.09f);

    name = prefix + idxStr + "].quadratic";
    shader->setFloat(name, 0.032f);


    // Set spot area values
    name = prefix + idxStr + "].cutOff";
    shader->setFloat(name, glm::cos(glm::radians(12.5f)));

    name = prefix + idxStr + "].outerCutOff";
    shader->setFloat(name, glm::cos(glm::radians(15.0f)));
}

void LightHandler::renderPointLight(int idx, Shader* shader, bool invertPos)
{
    // Return if max point light count
    if (idx >= 40)
        return;

    // Get light's mesh state
    RepeaterState* state = m_pointLights.at(idx)->mesh->getState();

    // Get light position
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    if (state->modified->size() > 0) {
        pos.x = state->modified->at(0)->transformations->xPos;
        pos.y = state->modified->at(0)->transformations->yPos;
        pos.z = state->modified->at(0)->transformations->zPos;
    }

    // Create uniform array location name
    std::string prefix = "pointLights[";
    std::string idxStr = std::to_string(idx);
    std::string name = prefix + idxStr + "].position";

    // Set light position
    if (invertPos) // Invert light position when rendering a model
        shader->setVec3(name, -pos);
    else
        shader->setVec3(name, pos);

    name = prefix + idxStr + "].ambient";
    shader->setVec3(name, glm::vec3(0.05f, 0.05f, 0.05f));

    name = prefix + idxStr + "].diffuse";
    shader->setVec3(name, glm::vec3(0.8f, 0.8f, 0.8f));

    name = prefix + idxStr + "].specular";
    shader->setVec3(name, glm::vec3(1.0f, 1.0f, 1.0f));


    // Set light fade-out values
    name = prefix + idxStr + "].constant";
    shader->setFloat(name, 1.0f);

    name = prefix + idxStr + "].linear";
    shader->setFloat(name, 0.09f);

    name = prefix + idxStr + "].quadratic";
    shader->setFloat(name, 0.032f);
}

void LightHandler::renderAllLightTypes(Shader* shader, bool invertPos)
{
    // Directional lights
    shader->setInt("dirLightCount", m_directionalLights.size());
    for (int i = 0; i < m_directionalLights.size(); i++) {
        renderDirectionalLight(i, shader, invertPos);
    }

    // Point lights
    shader->setInt("pointLightCount", m_pointLights.size());
    for (int i = 0; i < m_pointLights.size(); i++) {
        renderPointLight(i, shader, invertPos);
    }

    // Spotlights
    shader->setInt("spotLightCount", m_spotLights.size());
    for (int i = 0; i < m_spotLights.size(); i++) {
        renderSpotLight(i, shader, invertPos);
    }
}

void LightHandler::addPointLight(MeshObject* object)
{
    m_pointLights.push_back(object);
}

void LightHandler::addDirectionalLight(MeshObject* object)
{
    m_directionalLights.push_back(object);
}

void LightHandler::addSpotLight(MeshObject* object)
{
    m_spotLights.push_back(object);
}

void LightHandler::deletePointLight(std::string& name)
{
    for (int i = 0; i < m_pointLights.size(); i++) {
        if (!m_pointLights.at(i)->name.compare(name))
            m_pointLights.erase(m_pointLights.begin() + i);
    }
}

void LightHandler::deleteDirectionalLight(std::string& name)
{
    for (int i = 0; i < m_directionalLights.size(); i++) {
        if (!m_directionalLights.at(i)->name.compare(name))
            m_directionalLights.erase(m_directionalLights.begin() + i);
    }
}

void LightHandler::deleteSpotLight(std::string& name)
{
    for (int i = 0; i < m_spotLights.size(); i++) {
        if (!m_spotLights.at(i)->name.compare(name))
            m_spotLights.erase(m_spotLights.begin() + i);
    }
}