#ifndef MESH_LIST_HANDLER_H
#define MESH_LIST_HANDLER_H

#include "MeshObject.h"

#include <vector>

class MeshListHandler
{
public:
    MeshListHandler(std::vector<MeshObject*>* meshlist);
    ~MeshListHandler();

private:
    std::vector<MeshObject*>* m_meshList;
};

#endif // MESH_LIST_HANDLER_H