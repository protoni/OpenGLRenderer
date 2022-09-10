#include "Scene.h"

#include "stb_image.h"

float multiply = 1.0f;

#include "DebugMacros.h"

float hexagon_vertices[] = {
    -0.5f,  1.0f, 0.0f,  0.0f, 0.0f,  // top left
     0.5f,  1.0f, 0.0f,  1.0f, 0.0f,  // top right
     1.0f,  0.0f, 0.0f,  1.0f, 1.0f,  // center right
     0.5f, -1.0f, 0.0f,  1.0f, 1.0f,  // bottom right
    -0.5f, -1.0f, 0.0f,  0.0f, 1.0f,  // bottom left
    -1.0f,  0.0f, 0.0f,  0.0f, 0.0f,  // center left
     0.0f,  0.0f, 0.0f,  1.0f, 0.0f   // center
};

unsigned int hexagon_indices[] = {
    6, 0, 1, // top
    6, 1, 2, // top right
    6, 2, 3, // bottom right
    6, 3, 4, // bottom
    6, 4, 5, // bottom left
    6, 5, 0  // top left
};

Scene::Scene(Camera *camera, ScreenSettings* screenSettings) :
    m_camera(camera), m_screenSettings(screenSettings), m_faceCounter(3),
    m_ourShader(NULL), m_texture1(0), m_texture2(0), m_VAO(0), m_EBO(0),
    m_smiley_texture(NULL), m_columns(1), m_meshList(), m_scale(1.0), m_rows(1), m_instanced(false),
    m_ourShaderInstanced(NULL), m_instanced_cube(false), m_meshPointer(0)
{
    // Create and build shaders
    m_ourShaderInstanced = new Shader("./shaderInstanced.vs", "./shader.fs");
    m_ourShader = new Shader("./shader.vs", "./shader.fs");
    m_lightShader = new Shader("./shader.vs", "./lightShader.fs");
    m_lightMeshShader = new Shader("./lightMeshShader.vs", "./lightMeshShader.fs");
    
    // Load texture
    m_smiley_texture = new Texture("awesomeface.png");

    // Create mesh vector
    m_meshList = new std::vector<MeshObject*>;

}

Scene::~Scene()
{
    if (m_meshList) {
        delete m_meshList;
        m_meshList = NULL;
    }

    if (m_smiley_texture) {
        delete m_smiley_texture;
        m_smiley_texture = NULL;
    }

    if (m_ourShaderInstanced) {
        delete m_ourShaderInstanced;
        m_ourShaderInstanced = NULL;
    }

    if (m_ourShader) {
        delete m_ourShader;
        m_ourShader = NULL;
    }

    if (m_lightShader) {
        delete m_lightShader;
        m_lightShader = NULL;
    }

    if (m_lightMeshShader) {
        delete m_lightMeshShader;
        m_lightMeshShader = NULL;
    }
}

void Scene::updateMeshShader(bool instanced, int idx)
{
    if (instanced) {
        m_meshList->at(idx)->mesh->setShader(m_ourShaderInstanced);
    }
    else {
        m_meshList->at(idx)->mesh->setShader(m_ourShader);
    }

    m_meshList->at(idx)->mesh->setInstanced(instanced);
}

void Scene::addCube()
{
    Cube* cube = new Cube(m_ourShader, false, false, true);
    MeshObject* object = new MeshObject();
    object->mesh = cube;
    object->name = std::string("Cube_") + std::to_string(m_meshList->size());
    m_meshList->push_back(object);
}

void Scene::addPlane()
{
    Plane* plane = new Plane(m_ourShader, false);
    MeshObject* object = new MeshObject();
    object->mesh = plane;
    object->name = std::string("Plane_") + std::to_string(m_meshList->size());
    m_meshList->push_back(object);
}

void Scene::addTriangle()
{
    Triangle* triangle = new Triangle(m_ourShader, false);
    MeshObject* object = new MeshObject();
    object->mesh = triangle;
    object->name = std::string("Triangle_") + std::to_string(m_meshList->size());
    m_meshList->push_back(object);
}

void Scene::addSphere()
{
    Sphere* sphere = new Sphere(m_ourShader, false);
    MeshObject* object = new MeshObject();
    object->mesh = sphere;
    object->name = std::string("Sphere_") + std::to_string(m_meshList->size());
    m_meshList->push_back(object);
}

void Scene::addCustom()
{
    Custom* custom = new Custom(m_ourShader, false);
    MeshObject* object = new MeshObject();
    object->mesh = custom;
    object->name = std::string("Custom_") + std::to_string(m_meshList->size());
    m_meshList->push_back(object);
}

void Scene::addReflectingCube()
{
    Cube* cube = new Cube(m_lightMeshShader, false, false, true);
    MeshObject* object = new MeshObject();
    object->mesh = cube;
    object->name = std::string("ReflectingCube_") + std::to_string(m_meshList->size());
    object->type = MeshType::ReflectCubeType;
    m_meshList->push_back(object);
}

void Scene::addLight()
{
    Cube* cube = new Cube(m_lightShader, false, true, true);
    MeshObject* object = new MeshObject();
    object->mesh = cube;
    object->name = std::string("Light_") + std::to_string(m_meshList->size());
    object->type = MeshType::LightType;
    m_meshList->push_back(object);
}

void Scene::updateMeshPointer(int direction, bool multiselect)
{
    RepeaterState* state = m_meshList->at(getSelectedMeshIndex())->mesh->getState();

    // Figure out which stack are we on
    int stackPosition = 0;
    if (m_meshPointer > 0) {
        stackPosition = (m_meshPointer / (state->columnCount * state->rowCount));
    }

    // Figure out which row are we on
    int rowPosition = 0;
    if (m_meshPointer > 0) {
        if(stackPosition > 0)
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
        if(m_meshPointer - (rowPosition * state->columnCount) > 0)
            m_meshPointer -= 1;
    }
    else if (direction == MeshInstanceDirections::Right) {
        if (rowPosition > 0) {
            if (m_meshPointer - (rowPosition * state->columnCount) < state->rowCount - 1)
                m_meshPointer += 1;
        }
        else {
            if(m_meshPointer < state->columnCount - 1)
                m_meshPointer += 1;
        }
    }

    // Update Forward / Backward direction
    else if (direction == MeshInstanceDirections::Backward) {
        if(rowPosition < state->rowCount - 1)
            m_meshPointer += state->columnCount;
    }
    else if (direction == MeshInstanceDirections::Forward) {
        if (rowPosition > 0)
            m_meshPointer -= state->columnCount;
    }

    // Update Up / Down direction
    else if (direction == MeshInstanceDirections::Up) {
        //if (rowPosition < state->rowCount - 1)
        //if (m_meshPointer - (rowPosition * state->columnCount) < state->rowCount - 1)
        //std::cout << "move up"
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
                if(m_multiSelectVec.size() > i)
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
                if(temp.size() > i)
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

void Scene::resetMeshPointer()
{
    m_meshPointer = 0;
}

void Scene::deleteInstancedMesh(int selected)
{
    bool deleted = false;
    std::cout << "selected idx: " << getSelectedMeshIndex() << std::endl;
    if (getSelectedMeshIndex() < m_meshList->size()) {
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

        if(deleted)
            m_meshList->at(selected)->mesh->update();
    }
}

std::vector<MeshObject*>* Scene::getMeshList()
{
    return m_meshList;
}

int Scene::getSelectedMeshIndex()
{
    for (int i = 0; i < m_meshList->size(); i++) {
        if (m_meshList->at(i)->selected)
            return i;
    }

    return -1;
}

bool Scene::updateObjectMesh(int idx)
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

    m_meshList->at(idx)->mesh->update();

    std::cout << "Triangle count: " << getTriangleCount() << std::endl;

    return true;
}

int Scene::getTriangleCount()
{
    int count = 0;
    
    for (int i = 0; i < m_meshList->size(); i++) {
        count += m_meshList->at(i)->mesh->getObjCount() * m_meshList->at(i)->mesh->getIndexCount();
    }

    return count;
}

int Scene::getObjectCount()
{
    int count = 0;
    RepeaterState* state;
    for (int i = 0; i < m_meshList->size(); i++) {
        state = m_meshList->at(i)->mesh->getState();
        if (state->deleted)
            count += m_meshList->at(i)->mesh->getObjCount() - state->deleted->size();
        else
            count += m_meshList->at(i)->mesh->getObjCount();
    }

    return count;
}

void Scene::multiPick()
{
    if (m_meshPointer > 0) {
        if (std::find(m_multiSelectVec.begin(), m_multiSelectVec.end(), m_meshPointer) == m_multiSelectVec.end()) {
            m_multiSelectVec.push_back(m_meshPointer);
            std::cout << "add to multi pick vec!" << std::endl;
        }
    }
}

void Scene::highlightSelectedMeshes()
{
    glGenBuffers(1, &m_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    std::cout << "sending " << (sizeof(m_multiSelectVec) * m_multiSelectVec.size()) / 1024 << " kb to SSBO!" << std::endl;
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(m_multiSelectVec) * m_multiSelectVec.size(), &m_multiSelectVec[0], GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
    m_oldMultiSelectVecSize = m_multiSelectVec.size();
    m_oldMeshPointer = m_meshPointer;
}

void Scene::draw(int idx, glm::mat4& projection, glm::mat4& view)
{
    
    RepeaterState* state = m_meshList->at(idx)->mesh->getState();
    if (state->instanced) {
        m_ourShaderInstanced->use();
        m_ourShaderInstanced->setMat4("projection", projection);
        m_ourShaderInstanced->setMat4("view", view);
        m_ourShaderInstanced->setInt("selectedMesh", m_meshPointer);
        m_ourShaderInstanced->setInt("selectedInstance", m_meshList->at(idx)->selected);
        //if (m_meshList->at(i)->selected) {
        //    m_ourShaderInstanced->setVec4("selectColor", glm::vec4(0.2, 0.0, 0.0, 0.5));
        //}
        //else
        //    m_ourShaderInstanced->setVec4("selectColor", glm::vec4(0.0, 0.0, 0.0, 0.0));

        if (m_multiSelectVec.size() > 0) {
            if (m_multiSelectVec.size() != m_oldMultiSelectVecSize || m_oldMeshPointer != m_meshPointer) {
                highlightSelectedMeshes();
            }
        
        }
    }
    else {
        if (m_meshList->at(idx)->type == MeshType::ReflectCubeType) {
            m_lightMeshShader->use();
            m_lightMeshShader->setMat4("projection", projection);
            m_lightMeshShader->setMat4("view", view);
            m_lightMeshShader->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
            //m_lightMeshShader->setVec3("objectColor", glm::vec3(0.1f, 0.9f, 0.31f));
            m_lightMeshShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

            std::cout << "light pos: X=" << m_lightPos.x << ", Y=" << m_lightPos.y << ", Z=" << m_lightPos.z << std::endl;

            m_lightMeshShader->setVec3("lightPos", m_lightPos);
            m_lightMeshShader->setVec3("viewPos", m_camera->Position);

            // Set material
            //m_lightMeshShader->setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
            //m_lightMeshShader->setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
            //m_lightMeshShader->setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
            //m_lightMeshShader->setFloat("material.shininess", 32);
            // 
            // Set emerald material
            m_lightMeshShader->setVec3("material.ambient", Materials::materialTypeGreenRubber.ambient);
            m_lightMeshShader->setVec3("material.diffuse", Materials::materialTypeGreenRubber.diffuse);
            m_lightMeshShader->setVec3("material.specular", Materials::materialTypeGreenRubber.specular);
            m_lightMeshShader->setFloat("material.shininess", Materials::materialTypeGreenRubber.shininess);

            // Set lighting intensities for material
            m_lightMeshShader->setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            m_lightMeshShader->setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            m_lightMeshShader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        }
        else {
            m_ourShader->use();
            m_ourShader->setMat4("projection", projection);
            m_ourShader->setMat4("view", view);

            if (m_meshList->at(idx)->selected)
                m_ourShader->setVec4("selectColor", glm::vec4(0.2, 0.0, 0.0, 0.5));
            else
                m_ourShader->setVec4("selectColor", glm::vec4(0.0, 0.0, 0.0, 0.0));
        }

        if (m_meshList->at(idx)->type == MeshType::LightType) {
            m_lightShader->use();
            m_lightShader->setMat4("projection", projection);
            m_lightShader->setMat4("view", view);

            // Update light position
            if (state->modified->size() > 0) {
                m_lightPos.x = state->modified->at(0)->transformations->xPos;
                m_lightPos.y = state->modified->at(0)->transformations->yPos;
                m_lightPos.z = state->modified->at(0)->transformations->zPos;
            }

            

        }
    }

    m_meshList->at(idx)->mesh->draw();
}

void Scene::renderScene()
{
    if (!m_ourShader || !m_ourShaderInstanced) {
        std::cout << "Scene::renderScene::shader error!" << std::endl;
        return;
    }

    m_smiley_texture->use();

    glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), (float)m_screenSettings->width / (float)m_screenSettings->height, 0.1f, 100.0f);
    glm::mat4 view = m_camera->GetViewMatrix();

    int lastDrawn = 0;
    for (int i = 0; i < m_meshList->size(); i++) {
        //if (m_meshList->at(i)->selected)
        //    lastDrawn = i;
        //else
            draw(i, projection, view);
    }

    //if(lastDrawn < m_meshList->size())
    //    draw(lastDrawn, projection, view);
}

void Scene::update()
{
    m_faceCounter += 3;
    if (m_faceCounter >= 21) {
        m_faceCounter = 3;
    }
}

void Scene::deleteObject(int idx)
{
    // Clear individual deleted meshes inside an instance
    RepeaterState* state = m_meshList->at(idx)->mesh->getState();
    if (state->deleted) {
        delete state->deleted;
    }

    // Clear mesh pointer position
    if (state->position)
        delete state->position;

    // Clear mesh transformations
    if (state->transformations)
        delete state->transformations;

    // Cler modified mesh list
    if (state->modified) {
        //for (int i = 0; i < state->modified->size(); i++) {
        //    delete state->modified->at(i);
        //}

        // Clear old mesh transformations
        for (int i = 0; i < state->modified->size(); i++) {
            if (state->modified->at(i)) {
                if (state->modified->at(i)->position) {
                    delete state->modified->at(i)->position;
                    state->modified->at(i)->position = NULL;
                }
                if (state->modified->at(i)->transformations) {
                    delete state->modified->at(i)->transformations;
                    state->modified->at(i)->transformations = NULL;
                }
                if (state->modified->at(i)) {
                    delete state->modified->at(i);
                    state->modified->at(i) = NULL;
                }
            }
        }

        delete state->modified;
    }

    // Clear the mesh
    delete m_meshList->at(idx)->mesh;

    // Clear object
    delete m_meshList->at(idx);

    // Clear the object list
    m_meshList->erase(m_meshList->begin() + idx);
}

void Scene::clean()
{
    std::cout << "Cleaning: " << m_meshList->size() << " objects!" << std::endl;

    for (int i = m_meshList->size() -1; i >= 0; i--) {
        deleteObject(i);
    }

    m_meshList->clear();
}