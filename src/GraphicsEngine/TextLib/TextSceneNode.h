#pragma once

#include "cFont.h"

#include <IMeshData.h>
#include <ITexture.h>

#include <MathLib/math.h>

#include <string>

namespace ShiftEngine
{

class TextSceneNode
{
public:
    TextSceneNode(const std::string & text, MathLib::vec2f position, cFont * font);

    const std::string&  GetText() const;
    void                SetText(const std::string& text);

    IMeshDataPtr        GetMesh() const;
    ITexturePtr         GetTexture() const;

    MathLib::vec2f      GetPosition() const;
    void                SetPosition(MathLib::vec2f position);

    void                Kill();

private:
    void                CreateGeometry();

    MathLib::vec2f  position;
    std::string     text;
    IMeshDataPtr    mesh;
    bool            dead = false;
    cFont *         font = nullptr;

};

using TextSceneNodePtr = std::shared_ptr<TextSceneNode>;

}
