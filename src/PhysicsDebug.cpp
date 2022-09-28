#include "PhysicsDebug.h"


void BulletDebugDrawer_DeprecatedOpenGL::SetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix)
{
    glUseProgram(0); // Use Fixed-function pipeline (no shaders)
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(&pViewMatrix[0][0]);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&pProjectionMatrix[0][0]);
}

void BulletDebugDrawer_DeprecatedOpenGL::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    glColor3f(color.x(), color.y(), color.z());
    glBegin(GL_LINES);
    glVertex3f(from.x(), from.y(), from.z());
    glVertex3f(to.x(), to.y(), to.z());
    glEnd();
}
