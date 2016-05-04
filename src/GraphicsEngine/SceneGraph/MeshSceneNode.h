#pragma once

#include "../Material.h"
#include "../IMeshData.h"
#include "../RenderQueue.h"

#include "ISceneNode.h"

namespace ShiftEngine
{

class ISceneNode;
class CameraSceneNode;

class MeshSceneNode : public ISceneNode
{
public:
    MeshSceneNode(const IMeshDataPtr & _data, const Material * mat, SceneGraph * sceneGraph);
    virtual ~MeshSceneNode();

    virtual int Render();
    virtual void PushToRQ(RenderQueue & rq);

    virtual IMeshDataPtr GetDataPtr() const;
    virtual void SetDataPtr(IMeshDataPtr data);

    bool IsVisible() const;
    void SetVisibility(bool vis);

    ShiftEngine::Material * GetMaterialPtr();
    virtual void SetMaterial(const ShiftEngine::Material * val);

    virtual MathLib::AABB GetBBox() const override;

private:
    Material material;
    IMeshDataPtr Data = nullptr;
    bool isVisible = true;

};

}
