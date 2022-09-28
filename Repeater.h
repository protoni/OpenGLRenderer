#ifndef REPEATER_H
#define REPEATER_H

#include "Mesh.h"
#include "RepeaterState.h"
#include "Physics.h"
#include "MousePicker.h"

#include <vector>

// Limit updating physic collision box scaling to every 10th frame
#define PHYSICS_UPDATE_LIMIT 20

class Repeater : public Mesh
{
public:
    //Repeater(Shader* shader, bool instanced);
    Repeater(
        Shader* shader,
        bool instanced,
        float* vertices,
        unsigned int* indices,
        unsigned int verticeCount,
        unsigned int indiceCount,
        bool isLight,
        bool useNormals
    );
    Repeater(
        Shader* shader,
        bool instanced,
        bool isLight,
        bool useNormals
    );

    ~Repeater();

    void drawNonInstanced(Physics* physics, MousePicker* picker, Camera* camera);
    void drawInstanced();
    void draw(Physics* physics, MousePicker* picker, Camera* camera);
    void setIndiceCount(unsigned int count);

    void update();
    void createBuffer();
    int getObjCount();
    RepeaterState* getState();

    void setInstanced(bool instanced);

    // Print information about mesh state
    void printState();

    // Update physics of all meshes inside the repeater object
    void updateMeshPhysics(Physics* physics);

    // Get the count of physics objects inside the repeater instance
    int getPhysicsObjectCount() { return m_state->physicsObjects->size(); }

private:

    // Render all non instanced meshes
    void renderNonInstanced(
        int xPos, int yPos, int zPos,
        RepeaterState* state,
        unsigned int ptr,
        Physics* physics,
        bool& cleared,
        MousePicker* picker,
        bool& mouseOvered,
        Camera* camera
    );

    bool meshDeleted(int meshPointer);

    Shader* m_shader;
    RepeaterState* m_state;
    Physics* m_physics;

    unsigned int m_indiceCount;
    unsigned int m_buffer;
    glm::mat4* m_matrices;
    int* m_selected;

    unsigned int m_deleteRemoved;

    int m_oldObjectCount;

    bool m_useNormals;

    bool m_cleared = true;

    // Physics limiter
    int m_physicsUpdateLimiter = 0;

    // Store the currently mouse-overed meshes Z-axis value
    float m_mouseOveredZ = 0.0f;
    glm::vec3 m_mouseOveredPos = glm::vec3(0);

    float m_height = 0.0f;
};

#endif // REPEATER_H