#ifndef MESH_LIST_HANDLER_H
#define MESH_LIST_HANDLER_H

#include "MeshObject.h"

#include <vector>

/* All the possible operations on how to change the current scene */
enum MeshObjectChangeAction
{
    UpdateObject,
    ResetMeshPointer,
    UpdateMeshPointer,
    DeleteInstancedMesh,
    SetMultiPickMode,
    MultiPick
};

/* Holds info on how the scene should change */
struct MeshObjectChange
{
    // Selected object index
    int selectedMesh;

    // Is multiple meshes being selected
    bool multiselect;

    // Is multi-pick mode on ( select meshes with a CTRL key )
    bool multiPick;

    // Which way to move the mesh pointer
    int direction;

    // How to change the mesh object ( MeshObjectChangeAction )
    int action;

    MeshObjectChange()
        : selectedMesh(-1)
        , multiselect(false)
        , multiPick(false)
        , direction(-1)
        , action(-1) {}
};

/* Which way to move the mesh pointer */
enum MeshInstanceDirections
{
    Left,
    Right,
    Up,
    Down,
    Forward,
    Backward
};

class MeshListHandler
{
public:
    MeshListHandler(std::vector<MeshObject*>* meshlist);
    ~MeshListHandler();

    // Move selected pointer around to highlight an mesh
    void updateMeshPointer(int direction, bool multiselect);

    // Get currently selected mesh pointer
    int getSelectedMeshIndex();

    // Control multi-pick mode ( individual mesh picker )
    void toggleMultiPickMode() { m_multiPickMode = !m_multiPickMode; }
    void setMultiPickMode(bool mode) { m_multiPickMode = mode; }

    // Add currently selected mesh to multi select vector
    void multiPick();

    // Highlight selected meshes if the mesh pointer has moved
    void highlightChanged();

    // Highlight selected meshes by updating the SSBO on the GPU
    void highlightSelectedMeshes();

    // Delete a single mesh inside an repeater instance
    void deleteInstancedMesh(int selected);

    // Reset currently selected mesh pointer
    void resetMeshPointer() { m_meshPointer = -1; }

    // Apply mesh transformations to the selected mesh
    bool updateObjectMesh(int idx);

    // Count all objects in the scene and return the count
    int getObjectCount();

    // Count the total amount of triangles of the meshes on the Scene currently and return the count
    int getTriangleCount();

    // Get the current mesh pointer index
    int getMeshPointer() { return m_meshPointer; }

private:

    // All of the objects that the Scene has currently
    std::vector<MeshObject*>* m_meshList;

    // Is multi-pick mode on
    bool m_multiPickMode = false;

    // Currently selected mesh pointer list
    std::vector<int> m_multiSelectVec;

    // Store old size of the currently selected meshes vector to detect changes
    int m_oldMultiSelectVecSize = 0;

    // Store old mesh pointer to detect changes
    unsigned int m_oldMeshPointer = 0;

    // Currently selected mesh
    unsigned int m_meshPointer = 0;
};

#endif // MESH_LIST_HANDLER_H