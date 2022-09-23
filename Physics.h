#ifndef PHYSICS_H
#define PHYSICS_H

#include "btBulletDynamicsCommon.h"

#include "PhysicsDebug.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>



#include <vector>



class Physics
{
public:
    Physics();
    ~Physics();

    // Add new physics object
    void addObject(glm::quat orientation, glm::vec3 position, int ptr);

    // Has object got hit
    bool hasHit(glm::vec3& ray_origin, glm::vec3& ray_end);

    // Update physics
    void update(glm::mat4& projection, glm::mat4& view);

private:

    // Initialize Bullet physics engine
    bool init();

    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    btDefaultCollisionConfiguration* m_collisionConfiguration;
    
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher* m_dispatcher;
    
    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    btBroadphaseInterface* m_overlappingPairCache;
    
    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* m_solver;
    
    btDiscreteDynamicsWorld* m_dynamicsWorld;

    // Collision box
    btCollisionShape* m_boxCollisionShape;

    // Store all physics objects here
    std::vector<btRigidBody*> m_rigidBodies;

    // Debug drawer
    BulletDebugDrawer_DeprecatedOpenGL* m_mydebugdrawer;
};

#endif // PHYSICS_H