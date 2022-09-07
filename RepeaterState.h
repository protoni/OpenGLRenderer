#ifndef REPEATER_STATE_H
#define REPEATER_STATE_H

#include <vector>

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
        , xRotation(0.001)
        , yRotation(0.001)
        , zRotation(0.001)
    {}
};

struct ModifiedMesh
{
    int meshPointer;
    MeshTransformations* transformations;
    MeshPointerPosition* position;

    ModifiedMesh()
        : meshPointer(0)
        , transformations(nullptr)
        , position(nullptr) {}
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

    // Currently selected mesh positions
    MeshPointerPosition* position;

    // Mesh transformations ( scale, padding, offset, rotations)
    MeshTransformations* transformations;

    RepeaterState() :
        rowCount(1),
        columnCount(1),
        stackCount(1),
        deleted(nullptr),
        position(nullptr),
        transformations(nullptr),
        modified(nullptr),
        instanced(false) {}
};

#endif // #ifndef REPEATER_H