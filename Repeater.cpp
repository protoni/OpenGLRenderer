#include "Repeater.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "DebugMacros.h"


Repeater::Repeater(
    Shader* shader,
    bool instanced,
    float* vertices,
    unsigned int* indices,
    unsigned int verticeCount,
    unsigned int indiceCount,
    bool isLight,
    bool useNormals
    ): Mesh(shader,
        vertices,
        indices,
        verticeCount,
        indiceCount,
        isLight,
        useNormals
    ), m_buffer(0), m_matrices(NULL), m_shader(shader), m_state(),
    m_indiceCount(indiceCount), m_deleteRemoved(0), m_oldObjectCount(0),
    m_selected(NULL), m_useNormals(useNormals)
{
    std::cout << "vertex count: " << verticeCount << ", index count: " << indiceCount << std::endl;

    // Init state
    m_state = new RepeaterState();
    m_state->position = new MeshPointerPosition();
    m_state->transformations = new MeshTransformations();
    m_state->modified = new std::vector<ModifiedMesh*>();

    // Set initial instancing state
    m_state->instanced = instanced;

    // Init instance buffer
    if (m_state->instanced)
        createBuffer();
}

Repeater::Repeater(Shader* shader, bool instanced, bool isLight, bool useNormals)
    : Mesh(shader, isLight, useNormals), m_buffer(0), m_matrices(NULL), m_shader(shader), m_state(), m_indiceCount(0), m_oldObjectCount(0), m_selected(NULL)
{
    // Init state
    m_state = new RepeaterState();
    m_state->position = new MeshPointerPosition();

    // Set initial instancing state
    m_state->instanced = instanced;

    // Init instance buffer
    if (m_state->instanced)
        createBuffer();

    cleanup();
}

Repeater::~Repeater()
{
    std::cout << "Repeater destructor called!" << std::endl;
    if(m_matrices)
        delete[] m_matrices;

    if (m_state) {
        delete m_state;
        m_state = NULL;
    }
}

void Repeater::setIndiceCount(unsigned int count)
{
    m_indiceCount = count;
}

bool Repeater::meshDeleted(int meshPointer)
{
    bool ret = false;
    if (m_state->deleted) {
        std::vector<int> deletedMeshes = *m_state->deleted;
        if (std::find(deletedMeshes.begin(), deletedMeshes.end(), meshPointer) != deletedMeshes.end())
            ret = true;
    }

    return ret;
}

// Create instanced buffer on the GPU
void Repeater::createBuffer()
{   
    int ptr = 0;

    // Clear old mesh transformations if object count has changed
    if (m_oldObjectCount != getObjCount()) {
        for (int i = 0; i < m_state->modified->size(); i++) {
            if (m_state->modified->at(i)) {
                if (m_state->modified->at(i)->position) {
                    delete m_state->modified->at(i)->position;
                    m_state->modified->at(i)->position = NULL;
                }
                if (m_state->modified->at(i)->transformations) {
                    delete m_state->modified->at(i)->transformations;
                    m_state->modified->at(i)->transformations = NULL;
                }
                if (m_state->modified->at(i)) {
                    delete m_state->modified->at(i);
                    m_state->modified->at(i) = NULL;
                }
            }
        }
        m_state->modified->clear();
    }
    
    // Clear old matrix data
    delete[] m_matrices;

    // Get deleted mesh count
    int deletedCount = 0;
    if (m_state->deleted)
        deletedCount = m_state->deleted->size();

    //int meshPointer = 0;
    bool removedLastTime = false;
    m_matrices = new glm::mat4[getObjCount()];

    for (int y = 0; y < m_state->stackCount; y++) {          // stacks  ( y-axis )
        for (int z = 0; z < m_state->rowCount; z++) {        // rows    ( z axis )
            for (int x = 0; x < m_state->columnCount; x++) {  // columns ( x axis )
                
                // Create new modified mesh data if object count has changed
                if (m_oldObjectCount != getObjCount()) {
                    ModifiedMesh* modifiedMesh = new ModifiedMesh();
                    modifiedMesh->meshPointer = ptr;

                    MeshPointerPosition* meshPointerPosition = new MeshPointerPosition();
                    modifiedMesh->position = meshPointerPosition;
                    modifiedMesh->position->meshPointer = ptr;
                    modifiedMesh->position->stackPosition = y;
                    modifiedMesh->position->rowPosition = z;
                    modifiedMesh->position->columnPosition = x;

                    MeshTransformations* meshTransformations = new MeshTransformations();
                    modifiedMesh->transformations = meshTransformations;

                    m_state->modified->push_back(modifiedMesh);
                }

                // Get mesh object
                if (ptr < m_state->modified->size()) {
                    if (m_state->modified->at(ptr)->deleted) 
                        m_matrices[ptr] = glm::mat4(0.0f);
                    else 
                        m_matrices[ptr] = *getMesh(x, y, z, m_state, ptr);
                }
                else 
                    m_matrices[ptr] = *getMesh(x, y, z, m_state, ptr);
                
                ptr++;
            }
        }
    }

    m_oldObjectCount = getObjCount();

    std::cout << "planes: " << (getObjCount()) << std::endl;

    glGenBuffers(1, &m_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, (getObjCount()) * sizeof(glm::mat4), &m_matrices[0], GL_STATIC_DRAW);
    
    activate();
   glEnableVertexAttribArray(3);
   glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
   glEnableVertexAttribArray(4);
   glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
   glEnableVertexAttribArray(5);
   glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
   glEnableVertexAttribArray(6);
   glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
   
   glVertexAttribDivisor(3, 1);
   glVertexAttribDivisor(4, 1);
   glVertexAttribDivisor(5, 1);
   glVertexAttribDivisor(6, 1);

    deactivate();
}

int Repeater::getObjCount()
{
    return m_state->rowCount * m_state->columnCount * m_state->stackCount;
}

RepeaterState* Repeater::getState()
{
    return m_state;
}

void Repeater::printState()
{
    std::cout << "------- Repeater State ---------" << std::endl;
    std::cout << "Rows:    " << m_state->rowCount << std::endl;
    std::cout << "Columns: " << m_state->columnCount << std::endl;
    std::cout << "Stacks:  " << m_state->stackCount << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

void Repeater::update()
{
    if (m_state->instanced)
        createBuffer();
}

void Repeater::drawNonInstanced(Physics* physics)
{
    if (getIsLight())
        activateLight();
    else
        activate();

    int ptr = 0;

    // Clear old mesh transformations if object count has changed
    if (m_oldObjectCount != getObjCount()) {
        for (int i = 0; i < m_state->modified->size(); i++) {
            if (m_state->modified->at(i)) {
                if (m_state->modified->at(i)->position) {
                    delete m_state->modified->at(i)->position;
                    m_state->modified->at(i)->position = NULL;
                }
                if (m_state->modified->at(i)->transformations) {
                    delete m_state->modified->at(i)->transformations;
                    m_state->modified->at(i)->transformations = NULL;
                }
                if (m_state->modified->at(i)) {
                    delete m_state->modified->at(i);
                    m_state->modified->at(i) = NULL;
                }
            }
        }
        m_state->modified->clear();
        m_cleared = true;
    }

    for (int y = 0; y < m_state->stackCount; y++) {          // stacks  ( y-axis )
        for (int z = 0; z < m_state->rowCount; z++) {        // rows    ( z axis )
            for (int x = 0; x < m_state->columnCount; x++) { // columns ( x axis )
                
                // Create new modified mesh data if object count has changed
                if (m_oldObjectCount != getObjCount()) {
                    ModifiedMesh* modifiedMesh = new ModifiedMesh();
                    modifiedMesh->meshPointer = ptr;

                    MeshPointerPosition* meshPointerPosition = new MeshPointerPosition();
                    modifiedMesh->position = meshPointerPosition;
                    modifiedMesh->position->meshPointer = ptr;
                    modifiedMesh->position->stackPosition = y;
                    modifiedMesh->position->rowPosition = z;
                    modifiedMesh->position->columnPosition = x;

                    MeshTransformations* meshTransformations = new MeshTransformations();
                    modifiedMesh->transformations = meshTransformations;

                    m_state->modified->push_back(modifiedMesh);
                }

                // Update all physics objects if the repeater state has been updated. TODO: Limit update time ?
                //if (m_state->orientationUpdated) {
                    glm::vec3 position = glm::vec3(
                        m_state->modified->at(ptr)->transformations->xPos,
                        m_state->modified->at(ptr)->transformations->yPos,
                        m_state->modified->at(ptr)->transformations->zPos
                    );

                    physics->updateObject(m_state->modified->at(ptr)->transformations->orientation, position, ptr);
                    //std::cout << "Updated physics object: " << ptr << std::endl;

                //}

                render(x, y, z, m_state, ptr, physics, m_cleared);

                ptr++;
            }
        }
    }

    m_oldObjectCount = getObjCount();

    // Reset updated state
    m_state->countUpdated = false;
    m_state->orientationUpdated = false;

    deactivate();
}

void Repeater::drawInstanced()
{
    activate();
    int deletedCount = 0;
    if (m_state->deleted)
        deletedCount = m_state->deleted->size();
    //std::cout << "Render: " << getObjCount() << " objects!" << std::endl;
    glDrawElementsInstanced(GL_TRIANGLES, m_indiceCount, GL_UNSIGNED_INT, 0, getObjCount());
    deactivate();
}

void Repeater::setInstanced(bool instanced)
{
    m_state->instanced = instanced;
}
        
void Repeater::draw(Physics* physics)
{
    if (m_state->instanced)
        drawInstanced();
    else {
        drawNonInstanced(physics);
    }
}

