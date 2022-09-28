#ifndef MOUSE_PICKER_H
#define MOUSE_PICKER_H

#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MousePicker
{
public:
    MousePicker(Camera* camera);
    ~MousePicker();

    // Get current ray
    glm::vec3& getCurrentRay() { return m_currentRay; }

    // Convert 2d mouse position into 3d ray
    //glm::vec3& calculateMouseRay();
    void calculateMouseRay();

    // Update the view
    void update(glm::mat4& projection, glm::mat4& view);

    // Print the current ray
    void printRay();

    // Test if mouse cursor intersects with a model mesh
    bool testRayIntersection(glm::mat4& modelMatrix);

    // Get ray origin
    glm::vec3& getRayOrigin() { return m_ray_origin; }

    // Get ray direction
    glm::vec3& getRayDirection() { return m_ray_direction; }

private:
    
    Camera* m_camera;

    glm::vec3 m_currentRay;

    glm::mat4 m_projection;
    glm::mat4 m_view;

    // Ray size
    glm::vec3 m_aabb_min = glm::vec3(-0.033f, -0.033f, -0.033f);
    glm::vec3 m_aabb_max = glm::vec3( 0.033f,  0.033f,  0.033f);

    // Ray
    glm::vec3 m_ray_origin;
    glm::vec3 m_ray_direction;

};

#endif // MOUSE_PICKER_H