#include "Physics.h"

#include <iostream>

Physics::Physics()
{
    init();
}

Physics::~Physics()
{
    // Clean rigid bodies before dynamic world
    for (int i = 0; i < m_rigidBodies.size(); i++) {
        m_dynamicsWorld->removeRigidBody(m_rigidBodies.at(i));
        delete m_rigidBodies.at(i)->getMotionState();
        delete m_rigidBodies.at(i);
    }

    if (m_collisionConfiguration) {
        delete m_collisionConfiguration;
        m_collisionConfiguration = NULL;
    }

    if (m_dispatcher) {
        delete m_dispatcher;
        m_dispatcher = NULL;
    }

    if (m_overlappingPairCache) {
        delete m_overlappingPairCache;
        m_overlappingPairCache = NULL;
    }

    if (m_solver) {
        delete m_solver;
        m_solver = NULL;
    }

    if (m_dynamicsWorld) {
        delete m_dynamicsWorld;
        m_dynamicsWorld = NULL;
    }

    if (m_boxCollisionShape) {
        delete m_boxCollisionShape;
        m_boxCollisionShape = NULL;
    }

    if (m_mydebugdrawer) {
        delete m_mydebugdrawer;
        m_mydebugdrawer = NULL;
    }
}

bool Physics::init()
{

    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    
    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    m_overlappingPairCache = new btDbvtBroadphase();
    
    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_solver = new btSequentialImpulseConstraintSolver;
    
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

    // Create a collision box
    m_boxCollisionShape = new btBoxShape(btVector3(0.125f, 0.125f, 0.125f));
    
    m_mydebugdrawer = new BulletDebugDrawer_DeprecatedOpenGL();
    m_dynamicsWorld->setDebugDrawer(m_mydebugdrawer);
    m_dynamicsWorld->stepSimulation(1.f / 60.f);

    return true; // TODO: validate objects
}

bool Physics::isMouseOvered(int ptr)
{
    if (ptr == m_mouseOveredObject)
        return true;

    return false;
}

bool Physics::hasHit(glm::vec3& ray_origin, glm::vec3& ray_end)
{
    //std::cout << "ray_origin x: " << ray_origin.x << ", y: " << ray_origin.y << ", z: " << ray_origin.z << std::endl;
    //std::cout << "ray_end x: " << ray_end.x << ", y: " << ray_end.y << ", z: " << ray_end.z << std::endl;

    btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(ray_origin.x, ray_origin.y, ray_origin.z), btVector3(ray_end.x, ray_end.y, ray_end.z));
    m_dynamicsWorld->rayTest(btVector3(ray_origin.x, ray_origin.y, ray_origin.z), btVector3(ray_end.x, ray_end.y, ray_end.z), RayCallback);

    if (RayCallback.hasHit()) {
        //std::cout << "mesh hit: " << (size_t)RayCallback.m_collisionObject->getUserPointer() << std::endl;
        m_mouseOveredObject = (size_t)RayCallback.m_collisionObject->getUserPointer();
        return true;
    }
    else {
        m_mouseOveredObject = -1;
    }
    return false;
}

bool Physics::addObject(glm::quat orientation, glm::vec3 position, int ptr)
{
    if (ptr != m_rigidBodies.size()) {
        return false;
    }

    // Create new motion state
    btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(
        btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w),
        btVector3(position.x, position.y, position.z)
    ));

    // Set physics info
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        0,                  // mass, in kg. 0 -> Static object, will never move.
        motionstate,
        m_boxCollisionShape,  // collision shape of body
        btVector3(0, 0, 0)    // local inertia
    );

    // Create new rigid body
    btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);

    // Add rigid body to physics object list
    m_rigidBodies.push_back(rigidBody);

    // Add rigid body to the physics world
    m_dynamicsWorld->addRigidBody(rigidBody);

    // Set repeater position as the object's ID // TODO: multiple repeater objects breaks this model
    rigidBody->setUserPointer((void*)ptr);

    return true; // TODO: validate
}

bool Physics::updateObject(glm::quat orientation, glm::vec3 size, glm::vec3 position, int ptr)
{
    bool ret = false;

    if (ptr >= 0 && ptr < m_rigidBodies.size()) {
        
        // Update position
        m_rigidBodies.at(ptr)->proceedToTransform(btTransform(
            btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w),
            btVector3(position.x, position.y, position.z)
        ));

        // Update size
        m_boxCollisionShape->setLocalScaling(btVector3(size.x * 4, size.y * 4, size.z * 4)); // TODO: why * 4 ?
    
        // Update dynamics world
        m_dynamicsWorld->updateAabbs();
    }
    else
        std::cout << "Failed to update rigid body with index: " << ptr << std::endl;

    return ret;
}

void Physics::update(glm::mat4& projection, glm::mat4& view)
{
    // Handle collision box debug drawing
    if (m_debugMode) {

        // Update debug view
        m_mydebugdrawer->SetMatrices(view, projection);

        // Draw debug view
        m_dynamicsWorld->debugDrawWorld();
    }
}

void Physics::deleteObjects()
{
    for (int i = 0; i < m_rigidBodies.size(); i++) {
        m_dynamicsWorld->removeRigidBody(m_rigidBodies.at(i));
    }
}

