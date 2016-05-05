#include "D3D11MeshManager.h"

#include <Utilities/logger.hpp>
#include <Utilities/ut.h>

#include "../../ShiftEngine.h"
#include "../../Utils.h"

using namespace ShiftEngine;

D3D11MeshManager::D3D11MeshManager(ID3D11Device * pDevice)
    : pDevice(pDevice)
{}

IMeshDataPtr D3D11MeshManager::LoadMesh(const std::wstring & fileName)
{
    auto iter = meshesData.find(fileName);
    if (iter != meshesData.end())
    {
        return iter->second;
    }
    else
    {
        ID3D11DeviceContext *pImmediate = nullptr;
        pDevice->GetImmediateContext(&pImmediate);
        D3D11MeshDataPtr ptr = std::make_shared<D3D11MeshData>(nullptr, nullptr, pDevice, pImmediate);
        if (!Load(fileName, ptr.get()))
        {
            LOG_ERROR("Unable to load mesh", utils::narrow(fileName));
            return nullptr;
        }
        else
        {
            meshesData[fileName] = ptr;
            return ptr;
        }
    }
}

IMeshDataPtr D3D11MeshManager::CreateMeshFromVertices(const uint8_t * verticesData,
                                                      size_t verticesDataSize,
                                                      const std::vector<uint32_t> & indicesData,
                                                      const VertexSemantic * semantic,
                                                      const MathLib::AABB & bbox)
{
    if (!semantic)
        return LoadErrorMesh();

    auto vd = GetContextManager()->GetVertexDeclaration(*semantic);
    ID3D11DeviceContext *pImmediate = nullptr;
    pDevice->GetImmediateContext(&pImmediate);
    D3D11MeshDataPtr out = std::make_shared<D3D11MeshData>(nullptr, nullptr, pDevice, pImmediate);
    if (!out->CreateBuffers(false, verticesData, verticesDataSize, indicesData.data(), indicesData.size() * sizeof(uint32_t), semantic, vd, bbox))
    {
        LOG_ERROR("Unable to create mesh from vertices");
        return nullptr;
    }
    return out;
}

IMeshDataPtr D3D11MeshManager::LoadErrorMesh()
{
    if (!errorMesh)
        errorMesh = Utilities::createCube();
    return errorMesh;
}

bool D3D11MeshManager::Load(const std::wstring & filename, D3D11MeshData * mesh)
{
    SerializedLIM vertices;
    std::vector<uint32_t> indices;

    if (!Utilities::getVerticesFromFile(filename, vertices, indices))
        return false;

    if (vertices.position.empty())
        return false;

    UpdateVertices(vertices, indices);

    std::vector<uint8_t> vertexData;
    size_t resultSize = vertices.position.size() * sizeof(MathLib::vec3f) +
        vertices.normal.size() * sizeof(MathLib::vec3f) +
        vertices.texcoord.size() * sizeof(MathLib::vec2f) +
        vertices.colors.size() * sizeof(MathLib::vec3f);

    vertexData.resize(resultSize);

    uint8_t *pData = vertexData.data();
    for (size_t i = 0; i < vertices.position.size(); ++i)
    {
        memcpy(pData, &(vertices.position[i]), sizeof(MathLib::vec3f));
        pData += sizeof(MathLib::vec3f);
        if (vertices.normal.size() > 0)
        {
            memcpy(pData, &(vertices.normal[i]), sizeof(MathLib::vec3f));
            pData += sizeof(MathLib::vec3f);
        }

        if (vertices.texcoord.size() > 0)
        {
            memcpy(pData, &(vertices.texcoord[i]), sizeof(MathLib::vec2f));
            pData += sizeof(MathLib::vec2f);
        }

        if (vertices.colors.size() > 0)
        {
            memcpy(pData, &(vertices.colors[i]), sizeof(MathLib::vec3f));
            pData += sizeof(MathLib::vec3f);
        }
    }

    auto sem = CreateSemantic(vertices);
    auto declaration = GetContextManager()->GetVertexDeclaration(*sem);
    if (!declaration)
        declaration = GetContextManager()->GetVertexDeclaration(*sem);

    MathLib::AABB calculatedBBox;
    calculatedBBox.bMin = vertices.position[0];
    calculatedBBox.bMax = vertices.position[0];

    for (const MathLib::vec3f & pos : vertices.position)
    {
        if (pos.x < calculatedBBox.bMin.x) calculatedBBox.bMin.x = pos.x;
        if (pos.y < calculatedBBox.bMin.y) calculatedBBox.bMin.y = pos.y;
        if (pos.z < calculatedBBox.bMin.z) calculatedBBox.bMin.z = pos.z;

        if (pos.x > calculatedBBox.bMax.x) calculatedBBox.bMax.x = pos.x;
        if (pos.y > calculatedBBox.bMax.y) calculatedBBox.bMax.y = pos.y;
        if (pos.z > calculatedBBox.bMax.z) calculatedBBox.bMax.z = pos.z;
    }

    if (!mesh->CreateBuffers(true, vertexData.data(), vertexData.size(), indices.data(), indices.size() * sizeof(uint32_t), sem, declaration, calculatedBBox))
        return false;

    return true;
}

void D3D11MeshManager::UpdateVertices(SerializedLIM & vertices, std::vector<uint32_t>& indices) const
{
    for (size_t i = 0; i < vertices.position.size(); i++)
    {
        vertices.position[i].x *= -1;
        std::swap(vertices.position[i].y, vertices.position[i].z);

        vertices.normal[i].x *= -1;
        std::swap(vertices.normal[i].y, vertices.normal[i].z);
    }

    for (size_t i = 0; i < indices.size(); i += 3)
        std::swap(indices[i], indices[i + 1]);
}

const VertexSemantic * D3D11MeshManager::CreateSemantic(const SerializedLIM & vertices)
{
    VertexSemantic sem;
    sem.addSemantic(ET_FLOAT, 3, ES_Position);
    if (!vertices.normal.empty())
        sem.addSemantic(ET_FLOAT, 3, ES_Normal);
    if (!vertices.texcoord.empty())
        sem.addSemantic(ET_FLOAT, 2, ES_Texcoord);
    if (!vertices.colors.empty())
        sem.addSemantic(ET_FLOAT, 3, ES_Color);

    auto iter = semantics.find(sem);
    if (iter != semantics.end())
        return &(*iter);

    auto insert_pair = semantics.insert(sem);
    if (insert_pair.second)
    {
        return &(*insert_pair.first);
    }

    // crash I think
    // or return already inserted?
    return nullptr;
}
