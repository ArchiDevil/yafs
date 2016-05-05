#pragma once

#include "ISceneNode.h"

namespace ShiftEngine
{
    enum LightNodeType
    {
        LNT_Point,
        LNT_Directional,
        //LNT_Omni,
    };

    class LightSceneNode final : public ISceneNode
    {
    public:
        LightSceneNode(LightNodeType type, const MathLib::vec3f & color = { 1.0f, 1.0f, 1.0f });

        virtual void PushToRQ(RenderQueue & rq) override;
        void SetColor(const MathLib::vec3f & color);
        MathLib::vec3f GetColor() const;

        LightNodeType GetType() const;

        float GetRadius() const;
        void SetRadius(float val);

        MathLib::vec3f GetDirection() const;
        void SetDirection(const MathLib::vec3f & val);

        void SetActive(bool active);
        bool IsActive() const;

        virtual void KillSelf();
        virtual MathLib::AABB GetBBox() const override;

    private:
        MathLib::vec3f color = { 1.0f, 1.0f, 1.0 };
        const LightNodeType type;

        //for directional light source type
        MathLib::vec3f direction = { 1.0f, 0.0f, 0.0f };

        //for point light source type
        float radius = 100.0f;

        bool active = true;
    };
}
