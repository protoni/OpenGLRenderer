#ifndef REPEATER_STATE_H
#define REPEATER_STATE_H

#include <vector>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "btBulletDynamicsCommon.h"

struct MeshPointerPosition
{
    int meshPointer;
    int stackPosition;
    int rowPosition;
    int columnPosition;

    MeshPointerPosition()
        : meshPointer(-1)
        , stackPosition(-1)
        , rowPosition(-1)
        , columnPosition(-1)
    {}
};

struct MeshTransformations
{
    // Padding / space between object sides
    float paddingX;
    float paddingY;
    float paddingZ;

    // Object size
    float scaleX;
    float scaleY;
    float scaleZ;

    // Translation offsets
    float xOffset;
    float yOffset;
    float zOffset;

    // Rotation
    float angle;
    float xRotation;
    float yRotation;
    float zRotation;

    // Position
    float xPos;
    float yPos;
    float zPos;

    // Has the x, y, z coordinates already calculated once
    bool positionInitialized;

    // Current position.
    glm::vec3 position;

    // Mesh orientation
    glm::quat orientation;

    // Mesh size
    glm::vec3 size;

    MeshTransformations()
        : scaleX(0.5)
        , scaleY(0.5)
        , scaleZ(0.5)
        , paddingX(0.0)
        , paddingY(0.0)
        , paddingZ(0.0)
        , xOffset(0.0)
        , yOffset(0.0)
        , zOffset(0.0)
        , angle(0.0)
        , xRotation(0.000001)
        , yRotation(0.000001)
        , zRotation(0.000001)
        , xPos(0.0)
        , yPos(0.0)
        , zPos(0.0)
        , orientation(glm::quat())
        , size(glm::vec3(scaleX, scaleY, scaleZ))
        , position(glm::vec3(0.0, 0.0, 0.0))
    {}
};

struct ModifiedMesh
{
    int meshPointer;

    // Modified transformations of this mesh
    MeshTransformations* transformations;

    // What is the position of this mesh inside an object instance
    MeshPointerPosition* position;
    
    // Has this mesh been deleted
    bool deleted;

    // Has the physics been enabled on this object
    bool physics;

    // Is the mesh being simulated by physics engine currently
    bool simulated;

    // What is the index of the vector in physics world
    int physicsPointer;

    ModifiedMesh()
        : meshPointer(0)
        , transformations(nullptr)
        , deleted(false)
        , position(nullptr)
        , physics(false)
        , simulated(false)
        , physicsPointer(-1) {}
};

struct RepeaterState
{
    // Repeater settings
    int rowCount;
    int columnCount;
    int stackCount;

    // Are the meshes instanced
    bool instanced;

    // Deleted meshes
    std::vector<int>* deleted;

    // Modified meshes
    std::vector<ModifiedMesh*>* modified;

    // Store all physics objects here
    std::vector<btRigidBody*>* physicsObjects;

    // Currently selected mesh positions
    MeshPointerPosition* position;

    // Mesh transformations ( scale, padding, offset, rotations)
    MeshTransformations* transformations;

    // Has the repeater count state been updated
    bool countUpdated;

    // Has the repeater orientation state been updated
    bool orientationUpdated;

    // Mass of the mesh. Used in physics engine.
    float mass;

    RepeaterState()
        : rowCount(1)
        , columnCount(1)
        , stackCount(1)
        , deleted(nullptr)
        , position(nullptr)
        , transformations(nullptr)
        , modified(nullptr)
        , physicsObjects(nullptr)
        , instanced(false)
        , countUpdated(false)
        , orientationUpdated(false)
        , mass(0.0) {}
};

#endif // #ifndef REPEATER_H