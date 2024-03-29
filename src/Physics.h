#ifndef PHYSICS_H
#define PHYSICS_H

#include "btBulletDynamicsCommon.h"

#include "PhysicsDebug.h"
#include "RepeaterState.h"

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
    bool addObject(RepeaterState* state, int ptr);

    // Has object got hit
    bool hasHit(glm::vec3& ray_origin, glm::vec3& ray_end);

    // Update physics
    void update(glm::mat4& projection, glm::mat4& view);

    // Update single physics object with index ptr ID
    bool updateObject(RepeaterState* state, int ptr);

    // Get the current debug state
    bool getDebugModeOn() { return m_debugMode; }

    // Enable/Disable debug mode
    void setDebugModeOn(bool state) { m_debugMode = state;  }

    // Check if the mesh is being mouseovered
    bool isMouseOvered(int ptr);

    // Clear all physics objects
    void deleteObjects(RepeaterState* state);

    // Get physics simulated position
    bool getObjectPosition(RepeaterState* state, int ptr);

    // Clear physic objects with a given rigid body vector index
    void deleteObject(RepeaterState* state, int idx);

    // Check if requested ptr already exists in rigid bodies list and return last index if not
    bool physicsObjectExists(RepeaterState* state, int idx);


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
    //std::vector<btRigidBody*> m_rigidBodies;

    // Debug drawer
    BulletDebugDrawer_DeprecatedOpenGL* m_mydebugdrawer;

    // Do we want to draw the borders of physics collision shapes
    bool m_debugMode = false;

    // Currently mouseovered object
    int m_mouseOveredObject = -1;
};

#endif // PHYSICS_H