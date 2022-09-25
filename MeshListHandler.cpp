#include "MeshListHandler.h"

//#include <glad/glad.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

MeshListHandler::MeshListHandler(std::vector<MeshObject*>* meshlist) : m_meshList(meshlist)
{
}

MeshListHandler::~MeshListHandler()
{
}

void MeshListHandler::updateMeshPointer(int direction, bool multiselect)
{
    if (getSelectedMeshIndex() < 0)
        return;

    RepeaterState* state;

    if (m_meshList->at(getSelectedMeshIndex())->type == MeshType::ModelType)
        state = m_meshList->at(getSelectedMeshIndex())->model->getMeshList()->at(0)->getState();
    else
        state = m_meshList->at(getSelectedMeshIndex())->mesh->getState();

    // Figure out which stack are we on
    int stackPosition = 0;
    if (m_meshPointer > 0) {
        stackPosition = (m_meshPointer / (state->columnCount * state->rowCount));
    }

    // Figure out which row are we on
    int rowPosition = 0;
    if (m_meshPointer > 0) {
        if (stackPosition > 0)
            rowPosition = ((m_meshPointer - (stackPosition * (state->columnCount * state->rowCount))) / state->columnCount);
        else
            rowPosition = (m_meshPointer / state->columnCount);
    }

    // Figure out which column are we on

    state->position->meshPointer = m_meshPointer;
    state->position->stackPosition = stackPosition;
    state->position->rowPosition = rowPosition;
    state->position->columnPosition = 0;

    std::cout << "rowPosition: " << rowPosition << std::endl;
    std::cout << "stackPosition: " << stackPosition << std::endl;

    // Update Left / Right direction
    if (direction == MeshInstanceDirections::Left) {
        if (m_meshPointer - (rowPosition * state->columnCount) > 0)
            m_meshPointer -= 1;
    }
    else if (direction == MeshInstanceDirections::Right) {
        if (rowPosition > 0) {
            if (m_meshPointer - (rowPosition * state->columnCount) < state->rowCount - 1)
                m_meshPointer += 1;
        }
        else {
            if (m_meshPointer < state->columnCount - 1)
                m_meshPointer += 1;
        }
    }

    // Update Forward / Backward direction
    else if (direction == MeshInstanceDirections::Backward) {
        if (rowPosition < state->rowCount - 1)
            m_meshPointer += state->columnCount;
    }
    else if (direction == MeshInstanceDirections::Forward) {
        if (rowPosition > 0)
            m_meshPointer -= state->columnCount;
    }

    // Update Up / Down direction
    else if (direction == MeshInstanceDirections::Up) {
        if (stackPosition > 0) {
            if ((m_meshPointer - (state->columnCount * state->rowCount) * stackPosition) < state->stackCount - 1) {
                m_meshPointer += state->columnCount * state->rowCount;//state->stackCount;
            }
            else {
                std::cout << "dbg: " << (state->columnCount * state->rowCount) * stackPosition << std::endl;
            }
        }
        else {
            m_meshPointer += state->columnCount * state->rowCount;
        }
    }

    else if (direction == MeshInstanceDirections::Down) {
        m_meshPointer -= state->columnCount * state->rowCount;
    }

    if (m_meshPointer >= state->columnCount * state->rowCount * state->stackCount)
        m_meshPointer = (state->columnCount * state->rowCount * state->stackCount) - 1;

    if (multiselect) {
        if (std::find(m_multiSelectVec.begin(), m_multiSelectVec.end(), m_meshPointer) == m_multiSelectVec.end()) {
            m_multiSelectVec.push_back(m_meshPointer);
            std::cout << "add to multi select vec!" << std::endl;
        }
    }
    else {

        // Save multipick array before clearing
        std::vector<int> temp;
        if (m_multiPickMode) {
            for (int i = 0; i < m_multiSelectVec.size(); i++) {
                if (m_multiSelectVec.size() > i)
                    temp.push_back(m_multiSelectVec.at(i));
            }
        }

        // Clear all the object's fragment shader colors before clearing up the mesh pointer vector, because
        // otherwise fragment shader will keep the old mesh pointer values in it's SSBO for some reason.
        m_multiSelectVec.clear();
        for (int i = 0; i < getObjectCount(); i++) {
            m_multiSelectVec.push_back(m_meshPointer);
        }

        // Send the actual cleared mesh pointer values to the fragment shader as a SSBO
        if (m_multiSelectVec.size() > 0)
            highlightSelectedMeshes();

        // Clear mesh pointer vector and add only the current position to it
        m_multiSelectVec.clear();


        if (m_multiPickMode) {
            for (int i = 0; i < temp.size(); i++) {
                if (temp.size() > i)
                    m_multiSelectVec.push_back(temp.at(i));
            }
        }
        else {
            m_multiSelectVec.push_back(m_meshPointer);
        }

    }


    if (m_multiSelectVec.size() > 0) {
        std::cout << "mesh pointers: ";
        for (int i = 0; i < m_multiSelectVec.size(); i++)
            std::cout << m_multiSelectVec.at(i) << ", ";

        std::cout << std::endl;
    }
    else {
        std::cout << "mesh pointer: " << m_meshPointer << std::endl;
    }
}

int MeshListHandler::getSelectedMeshIndex()
{
    for (int i = 0; i < m_meshList->size(); i++) {
        if (m_meshList->at(i)->selected)
            return i;
    }

    return -1;
}

void MeshListHandler::multiPick()
{
    if (m_meshPointer > 0) {
        if (std::find(m_multiSelectVec.begin(), m_multiSelectVec.end(), m_meshPointer) == m_multiSelectVec.end()) {
            m_multiSelectVec.push_back(m_meshPointer);
            std::cout << "add to multi pick vec!" << std::endl;
        }
    }
}

void MeshListHandler::highlightSelectedMeshes()
{
    GLuint ssbo;

    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    std::cout << "sending " << (sizeof(m_multiSelectVec) * m_multiSelectVec.size()) / 1024 << " kb to SSBO!" << std::endl;
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(m_multiSelectVec) * m_multiSelectVec.size(), &m_multiSelectVec[0], GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
    m_oldMultiSelectVecSize = m_multiSelectVec.size();
    m_oldMeshPointer = m_meshPointer;
}

void MeshListHandler::highlightChanged()
{
    if (m_multiSelectVec.size() > 0) {
        if (m_multiSelectVec.size() != m_oldMultiSelectVecSize || m_oldMeshPointer != m_meshPointer) {
            highlightSelectedMeshes();
        }

    }
}

void MeshListHandler::deleteInstancedMesh(int selected)
{
    bool deleted = false;
    std::cout << "selected idx: " << getSelectedMeshIndex() << std::endl;
    if (getSelectedMeshIndex() >= 0 && getSelectedMeshIndex() < m_meshList->size()) {
        RepeaterState* state = m_meshList->at(getSelectedMeshIndex())->mesh->getState();

        // Delete single mesh
        if (m_meshPointer >= 0 && m_meshPointer < state->modified->size()) {
            state->modified->at(m_meshPointer)->deleted = true;
            deleted = true;
            std::cout << "deleted mesh pointer: " << m_meshPointer << std::endl;
        }

        // Delete multi-selected meshes
        for (int i = 0; i < m_multiSelectVec.size(); i++) {
            if (m_multiSelectVec.at(i) >= 0 && m_multiSelectVec.at(i) < state->modified->size()) {
                state->modified->at(m_multiSelectVec.at(i))->deleted = true;
                deleted = true;
                std::cout << "deleted mesh pointer: " << m_multiSelectVec.at(i) << std::endl;
            }
        }

        if (deleted)
            m_meshList->at(selected)->mesh->update();
    }
}

int MeshListHandler::getObjectCount()
{
    int count = 0;
    RepeaterState* state;
    for (int i = 0; i < m_meshList->size(); i++) {
        if (m_meshList->at(i)->type == MeshType::ModelType) {
            count += m_meshList->at(i)->model->getObjectCount();
        }
        else {
            state = m_meshList->at(i)->mesh->getState();
            if (state->deleted)
                count += m_meshList->at(i)->mesh->getObjCount() - state->deleted->size();
            else
                count += m_meshList->at(i)->mesh->getObjCount();
        }
    }

    return count;
}

bool MeshListHandler::updateObjectMesh(int idx)
{
    if (!m_meshList->size() > idx) {
        return false;
    }

    // If multiple meshes were selected, apply the same changes to all of those
    if (m_multiSelectVec.size() > 0) {
        RepeaterState* state = m_meshList->at(idx)->mesh->getState();
        for (int i = 0; i < m_multiSelectVec.size(); i++) {

            // Get the transformations from the last item of the multi select vector, which is being modified currently
            MeshTransformations* src = state->modified->at(m_multiSelectVec.back())->transformations;

            // Create a new destination transformations struct and copy the modified transformations to this
            MeshTransformations* dst = new MeshTransformations();//state->modified->at(m_multiSelectVec.at(i))->transformations;
            memcpy(dst, src, sizeof(*src));

            // Free old transformations after copying
            delete state->modified->at(m_multiSelectVec.at(i))->transformations;

            // Assign copied transformations to the newly created struct
            state->modified->at(m_multiSelectVec.at(i))->transformations = dst;
        }
    }

    if (m_meshList->at(idx)->type == MeshType::ModelType) {
        m_meshList->at(idx)->model->update();
    }
    else {
        m_meshList->at(idx)->mesh->update();
        m_meshList->at(idx)->mesh->printState();
    }
    

    std::cout << "Triangle count: " << getTriangleCount() << std::endl;

    return true;
}

int MeshListHandler::getTriangleCount()
{
    int count = 0;

    for (int i = 0; i < m_meshList->size(); i++) {
        if (m_meshList->at(i)->type == MeshType::ModelType) {
            count += m_meshList->at(i)->model->getTriangleCount();
        }
        else
            count += m_meshList->at(i)->mesh->getObjCount() * m_meshList->at(i)->mesh->getIndexCount();
    }

    return count;
}

