#include "MeshSceneNode.h"

#include "../ShiftEngine.h"

using namespace ShiftEngine;
using namespace MathLib;

MeshSceneNode::MeshSceneNode(const IMeshDataPtr & _data, const Material * mat, SceneGraph * sceneGraph)
    : ISceneNode(sceneGraph)
    , isVisible(true)
    , material(*mat)
    , Data(_data)
{}

void MeshSceneNode::PushToRQ(RenderQueue & rq)
{
    int visibility = 1;//CheckVisibility(rq.GetActiveCamera());

    if (IsVisible() && visibility)
        rq.AddRenderableNode(this);
}

bool MeshSceneNode::IsVisible() const
{
    return isVisible;
}

void MeshSceneNode::SetVisibility(bool vis)
{
    isVisible = vis;
}

Material * MeshSceneNode::GetMaterialPtr()
{
    return &material;
}

void MeshSceneNode::SetMaterial(const Material * val)
{
    material = Material(*val);
}

IMeshDataPtr MeshSceneNode::GetDataPtr() const
{
    return Data;
}

void MeshSceneNode::SetDataPtr(IMeshDataPtr data)
{
    Data = data;
}

int MeshSceneNode::Render()
{
    if (!Data)
        return 0;
    return GetContextManager()->DrawMesh(Data);
}

AABB MeshSceneNode::GetBBox() const
{
    mat4f matWorld = GetWorldMatrix();
    vec4f points[8];
    AABB bbox = {};
    if (Data)
        bbox = Data->GetBBox();

    points[0] = {bbox.bMin.x, bbox.bMin.y, bbox.bMin.z, 1.0f};
    points[1] = {bbox.bMin.x, bbox.bMin.y, bbox.bMax.z, 1.0f};
    points[2] = {bbox.bMin.x, bbox.bMax.y, bbox.bMin.z, 1.0f};
    points[3] = {bbox.bMax.x, bbox.bMin.y, bbox.bMin.z, 1.0f};
    points[4] = {bbox.bMax.x, bbox.bMax.y, bbox.bMin.z, 1.0f};
    points[5] = {bbox.bMin.x, bbox.bMax.y, bbox.bMax.z, 1.0f};
    points[6] = {bbox.bMax.x, bbox.bMin.y, bbox.bMax.z, 1.0f};
    points[7] = {bbox.bMax.x, bbox.bMax.y, bbox.bMax.z, 1.0f};

    vec3f min, max;

    for (int i = 0; i < 8; i++)
        points[i] = vec4Transform(points[i], matWorld);

    min.x = points[0].x;
    min.y = points[0].y;
    min.z = points[0].z;
    max.x = points[0].x;
    max.y = points[0].y;
    max.z = points[0].z;
    for (int i = 0; i < 8; i++)
    {
        if (points[i].x < min.x)
            min.x = points[i].x;
        if (points[i].y < min.y)
            min.y = points[i].y;
        if (points[i].z < min.z)
            min.z = points[i].z;

        if (points[i].x > max.x)
            max.x = points[i].x;
        if (points[i].y > max.y)
            max.y = points[i].y;
        if (points[i].z > max.z)
            max.z = points[i].z;
    }

    return {{min.x, min.y, min.z}, {max.x, max.y, max.z}};
}
