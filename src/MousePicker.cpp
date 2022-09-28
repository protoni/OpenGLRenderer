#include "MousePicker.h"


#include <iostream>

MousePicker::MousePicker(Camera* camera)
    : m_camera(camera)
{

}

MousePicker::~MousePicker()
{
}

//glm::vec3& MousePicker::calculateMouseRay()
void MousePicker::calculateMouseRay()
{
    // Get mouse position and screen size
    glm::vec2 mousePos = m_camera->getMousePos();
    glm::vec2 windowSize = m_camera->getWindowSize();
    float mouseX = (float)mousePos.x;
    float mouseY = (float)mousePos.y;
    
    // Ray start position ( normalized device coordinates )
    glm::vec4 lRayStart_NDC(
        ((float)mouseX / (float)windowSize.x - 0.5f) * 2.0f,
        -((float)mouseY / (float)windowSize.y - 0.5f) * 2.0f, // Inverted y-axis
        -1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
        1.0f
    );

    // Ray end position ( normalized device coordinates )
    glm::vec4 lRayEnd_NDC(
        ((float)mouseX / (float)windowSize.x - 0.5f) * 2.0f,
        -((float)mouseY / (float)windowSize.y - 0.5f) * 2.0f, // Inverted y-axis
        0.0,
        1.0f
    );

    // Calculate ray start and end positions in the world
    glm::mat4 M = glm::inverse(m_projection * m_view);
    glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
    glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;

    glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
    lRayDir_world = glm::normalize(lRayDir_world);

    m_ray_origin = glm::vec3(lRayStart_world);
    m_ray_direction = glm::normalize(lRayDir_world);
}

void MousePicker::update(glm::mat4& projection, glm::mat4& view)
{
    m_projection = projection;
    m_view = view;
    //m_currentRay = calculateMouseRay();
    calculateMouseRay();
}

void MousePicker::printRay()
{
    glm::vec3 ray = getCurrentRay();
    std::cout << "Ray x: " << ray.x << ", y: " << ray.y << ", z: " << ray.z << std::endl;
}

bool MousePicker::testRayIntersection(glm::mat4& modelMatrix)
{
    // Intersection method from Real-Time Rendering and Essential Mathematics for Games
    //glm::vec3 ray = m_currentRay;
    //glm::vec3 ray_origin = m_currentRay;
    //glm::vec3 ray_direction = glm::normalize(m_currentRay);

    //std::cout << "m_ray_origin x: " << m_ray_origin.x << ", y: " << m_ray_origin.y << ", z: " << m_ray_origin.z << std::endl;
    //std::cout << "m_ray_direction x: " << m_ray_direction.x << ", y: " << m_ray_direction.y << ", z: " << m_ray_direction.z << std::endl;
    

    float tMin = 0.0f;
    float tMax = 100000.0f;

    glm::vec3 OBBposition_worldspace(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);

    glm::vec3 delta = OBBposition_worldspace - m_ray_origin;
    //std::cout << "delta x: " << delta.x << ", y: " << delta.y << ", z: " << delta.z << std::endl;

    // Test intersection with the 2 planes perpendicular to the OBB's X axis
    {
        glm::vec3 xaxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
        float e = glm::dot(xaxis, delta);
        float f = glm::dot(m_ray_direction, xaxis);
            
        if (fabs(f) > 0.001f) { // Standard case

            float t1 = (e + m_aabb_min.x) / f; // Intersection with the "left" plane
            float t2 = (e + m_aabb_max.x) / f; // Intersection with the "right" plane
            // t1 and t2 now contain distances betwen ray origin and ray-plane intersections

            // We want t1 to represent the nearest intersection, 
            // so if it's not the case, invert t1 and t2
            if (t1 > t2) {
                float w = t1; t1 = t2; t2 = w; // swap t1 and t2
            }

            // tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
            if (t2 < tMax)
                tMax = t2;
            // tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
            if (t1 > tMin)
                tMin = t1;

            // And here's the trick :
            // If "far" is closer than "near", then there is NO intersection.
            // See the images in the tutorials for the visual explanation.
            if (tMax < tMin)
                return false;

        }
        else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
            if (-e + m_aabb_min.x > 0.0f || -e + m_aabb_max.x < 0.0f)
                return false;
        }
    }


    // Test intersection with the 2 planes perpendicular to the OBB's Y axis
    // Exactly the same thing than above.
    {
        glm::vec3 yaxis(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
        float e = glm::dot(yaxis, delta);
        float f = glm::dot(m_ray_direction, yaxis);

        if (fabs(f) > 0.001f) {

            float t1 = (e + m_aabb_min.y) / f;
            float t2 = (e + m_aabb_max.y) / f;

            if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

            if (t2 < tMax)
                tMax = t2;
            if (t1 > tMin)
                tMin = t1;
            if (tMin > tMax)
                return false;

        }
        else {
            if (-e + m_aabb_min.y > 0.0f || -e + m_aabb_max.y < 0.0f)
                return false;
        }
    }


    // Test intersection with the 2 planes perpendicular to the OBB's Z axis
    // Exactly the same thing than above.
    {
        glm::vec3 zaxis(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);
        float e = glm::dot(zaxis, delta);
        float f = glm::dot(m_ray_direction, zaxis);

        if (fabs(f) > 0.001f) {

            float t1 = (e + m_aabb_min.z) / f;
            float t2 = (e + m_aabb_max.z) / f;

            if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

            if (t2 < tMax)
                tMax = t2;
            if (t1 > tMin)
                tMin = t1;
            if (tMin > tMax)
                return false;

        }
        else {
            if (-e + m_aabb_min.z > 0.0f || -e + m_aabb_max.z < 0.0f)
                return false;
        }
    }

    //intersection_distance = tMin;
    return true;
}

