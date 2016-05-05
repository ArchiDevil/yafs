#pragma once

#include "ISceneNode.h"

namespace ShiftEngine
{

class PlainTreeNode : public ISceneNode
{
public:
    PlainTreeNode(SceneGraph * sceneGraph)
        : ISceneNode(sceneGraph)
    {}

    virtual MathLib::AABB GetBBox() const override
    {
        return MathLib::AABB();
    }

protected:
    //just stub only for plain scene graph
    virtual void PushToRQ(RenderQueue&)
    {}
};

}
