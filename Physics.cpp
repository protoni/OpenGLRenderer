#include "Physics.h"

#include <iostream>

Physics::Physics()
{
    init();
}

Physics::~Physics()
{
    // Clean rigid bodies before dynamic world
    //for (int i = 0; i < m_rigidBodies.size(); i++) {
    //    m_dynamicsWorld->removeRigidBody(m_rigidBodies.at(i));
    //    delete m_rigidBodies.at(i)->getMotionState();
    //    delete m_rigidBodies.at(i);
    //}

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
    m_dynamicsWorld->setGravity(btVector3(0, -1.0f, 0)); // Default: -9.81f

    // Create a collision box
    m_boxCollisionShape = new btBoxShape(btVector3(0.125f, 0.125f, 0.125f));
    
    m_mydebugdrawer = new BulletDebugDrawer_DeprecatedOpenGL();
    m_dynamicsWorld->setDebugDrawer(m_mydebugdrawer);
    
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

bool Physics::addObject(RepeaterState* state, int ptr)
{
    if (state->modified->at(ptr)->physicsPointer != state->physicsObjects->size()) {
        std::cout << "Physics object exists already!" << std::endl;
        return false;
    }

    // Create new motion state
    btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(
        btQuaternion(
            state->modified->at(ptr)->transformations->orientation.x,
            state->modified->at(ptr)->transformations->orientation.y,
            state->modified->at(ptr)->transformations->orientation.z,
            state->modified->at(ptr)->transformations->orientation.w
        ),
        btVector3(
            state->modified->at(ptr)->transformations->xPos,
            state->modified->at(ptr)->transformations->yPos,
            state->modified->at(ptr)->transformations->zPos)
    ));

    // Set physics info
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        state->mass,                  // mass, in kg. 0 -> Static object, will never move.
        motionstate,
        m_boxCollisionShape,  // collision shape of body
        btVector3(0, 0, 0)    // local inertia
    );

    // Create new rigid body
    btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);

    // Add rigid body to physics object list
    state->physicsObjects->push_back(rigidBody);

    // Add rigid body to the physics world
    m_dynamicsWorld->addRigidBody(rigidBody);

    // Set repeater position as the object's ID // TODO: multiple repeater objects breaks this model
    rigidBody->setUserPointer((void*)state->modified->at(ptr)->physicsPointer);

    return true; // TODO: validate
}

bool Physics::updateObject(RepeaterState* state, int ptr)
{
    bool ret = false;

    if (state->modified->at(ptr)->physicsPointer >= 0 && state->modified->at(ptr)->physicsPointer < state->physicsObjects->size()) {
        
        // Update position
        state->physicsObjects->at(ptr)->proceedToTransform(btTransform(
            btQuaternion(
                state->modified->at(ptr)->transformations->orientation.x,
                state->modified->at(ptr)->transformations->orientation.y,
                state->modified->at(ptr)->transformations->orientation.z,
                state->modified->at(ptr)->transformations->orientation.w
            ),
            btVector3(
                state->modified->at(ptr)->transformations->xPos,
                state->modified->at(ptr)->transformations->yPos,
                state->modified->at(ptr)->transformations->zPos
            )
        ));

        // Update size
        m_boxCollisionShape->setLocalScaling(btVector3(
            state->modified->at(ptr)->transformations->size.x * 4,
            state->modified->at(ptr)->transformations->size.y * 4,
            state->modified->at(ptr)->transformations->size.z * 4
        )); // TODO: why * 4 ?
        
        // Update mass
        state->physicsObjects->at(state->modified->at(ptr)->physicsPointer)->setMassProps(state->mass, btVector3(0, 0, 0));

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

    m_dynamicsWorld->stepSimulation(1.f / 60.f, 1000);
}

bool Physics::getObjectPosition(RepeaterState* state, int ptr)
{
    if (state->modified->at(ptr)->physicsPointer >= state->physicsObjects->size() || ptr < 0) {
        std::cout << "Failed to get physics object position! Invalid index: " << ptr << std::endl;
        return false;
    }

    // Apply position
    state->modified->at(ptr)->transformations->position.x = 
        state->physicsObjects->at(state->modified->at(ptr)->physicsPointer)->getWorldTransform().getOrigin().x();
    state->modified->at(ptr)->transformations->position.y = 
        state->physicsObjects->at(state->modified->at(ptr)->physicsPointer)->getWorldTransform().getOrigin().y();
    state->modified->at(ptr)->transformations->position.z = 
        state->physicsObjects->at(state->modified->at(ptr)->physicsPointer)->getWorldTransform().getOrigin().z();
    state->modified->at(ptr)->simulated = true;

    // Apply rotation // TODO: Fix. collider box rotation doesn't sync with actual mesh transformations
    state->modified->at(ptr)->transformations->orientation.x = 
        state->physicsObjects->at(state->modified->at(ptr)->physicsPointer)->getWorldTransform().getRotation().x();
    state->modified->at(ptr)->transformations->orientation.y = 
        state->physicsObjects->at(state->modified->at(ptr)->physicsPointer)->getWorldTransform().getRotation().y();
    state->modified->at(ptr)->transformations->orientation.z = 
        state->physicsObjects->at(state->modified->at(ptr)->physicsPointer)->getWorldTransform().getRotation().z();
    state->modified->at(ptr)->transformations->orientation.w = 
        state->physicsObjects->at(state->modified->at(ptr)->physicsPointer)->getWorldTransform().getRotation().w();

    return true;
}

void Physics::deleteObjects(RepeaterState* state)
{
    for (int i = 0; i < state->physicsObjects->size(); i++) {
        m_dynamicsWorld->removeRigidBody(state->physicsObjects->at(i));
        delete state->physicsObjects->at(i)->getMotionState();
        delete state->physicsObjects->at(i);
    }
    state->physicsObjects->clear();
}

void Physics::deleteObject(RepeaterState* state, int idx)
{
    if (idx < state->physicsObjects->size()) {
        m_dynamicsWorld->removeRigidBody(state->physicsObjects->at(idx));
        delete state->physicsObjects->at(idx)->getMotionState();
        delete state->physicsObjects->at(idx);
        state->physicsObjects->erase(state->physicsObjects->begin() + idx, state->physicsObjects->end());
    }
}

bool Physics::physicsObjectExists(RepeaterState* state, int ptr)
{
    if (ptr >= 0 && ptr < state->physicsObjects->size()) {
        return true;
    }

    return false;
}


