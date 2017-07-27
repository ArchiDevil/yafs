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

    virtual MathLib::aabb3f GetBBox() const override
    {
        return {};
    }

protected:
    //just stub only for plain scene graph
    virtual void PushToRQ(RenderQueue&)
    {}
};

}
