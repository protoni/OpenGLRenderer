#ifndef PHYSICS_DEBUG_H
#define PHYSICS_DEBUG_H

#include "btBulletDynamicsCommon.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class BulletDebugDrawer_DeprecatedOpenGL : public btIDebugDraw {
public:
    void SetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix);
    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
    virtual void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) {}
    virtual void reportErrorWarning(const char*) {}
    virtual void draw3dText(const btVector3&, const char*) {}
    virtual void setDebugMode(int p) {
        m = p;
    }
    int getDebugMode(void) const { return 3; }
    int m;
};

#endif // PHYSICS_DEBUG_H