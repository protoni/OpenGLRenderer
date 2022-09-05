#ifndef REPEATER_STATE_H
#define REPEATER_STATE_H

struct RepeaterState
{
    // Repeater settings
    int rowCount;
    int columnCount;
    int stackCount;

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

    bool instanced;

    unsigned int selected;

    RepeaterState() :
        rowCount(1),
        columnCount(1),
        stackCount(1),
        scaleX(0.5),
        scaleY(0.5),
        scaleZ(0.5),
        paddingX(0.0),
        paddingY(0.0),
        paddingZ(0.0),
        xOffset(0.0),
        yOffset(0.0),
        zOffset(0.0),
        angle(0.0),
        xRotation(0.001),
        yRotation(0.001),
        zRotation(0.001),
        selected(0),
        instanced(false) {}
};

#endif // #ifndef REPEATER_H