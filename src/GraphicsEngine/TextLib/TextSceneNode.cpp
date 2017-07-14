#include "TextSceneNode.h"

#include "ShiftEngine.h"

#include <cassert>

using namespace ShiftEngine;

TextSceneNode::TextSceneNode(const std::string & text, MathLib::vec2f position, cFont * font)
    : text(text)
    , position(position)
    , font(font)
{
    CreateGeometry();
}

const std::string& TextSceneNode::GetText() const
{
    return text;
}

IMeshDataPtr TextSceneNode::GetMesh() const
{
    return mesh;
}

void TextSceneNode::Kill()
{
    dead = true;
}

MathLib::vec2f TextSceneNode::GetPosition() const
{
    return position;
}

void TextSceneNode::SetPosition(MathLib::vec2f pos)
{
    this->position = pos;
}

void ShiftEngine::TextSceneNode::SetText(const std::string& t)
{
    this->text = t;
    CreateGeometry();
}

void ShiftEngine::TextSceneNode::CreateGeometry()
{
    if (text.empty())
        return;

    std::vector<SpriteVertex> v;
    std::vector<uint32_t> i;

    i.resize(text.size() * 6);
    v.resize(text.size() * 4);

    int curX = 0;
    uint32_t vertSh = 0;
    uint32_t indSh = 0;

    for (char symbol : text)
    {
        sChar * cp = font->GetCharacterPtr(symbol);
        assert(cp->Height != 0);
        int CharX = cp->x;
        int CharY = cp->y;
        int Width = cp->Width;
        int Height = cp->Height;
        int OffsetX = cp->XOffset;
        int OffsetY = cp->YOffset;

        //first triangle
        i[indSh++] = vertSh;
        i[indSh++] = vertSh + 1;
        i[indSh++] = vertSh + 2;

        //second triangle
        i[indSh++] = vertSh;
        i[indSh++] = vertSh + 2;
        i[indSh++] = vertSh + 3;

        /*
        0   1
        -----
        |\  |
        | \ |
        |  \|
        -----
        3   2
        */

        //upper left
        v[vertSh].texcoord[0] = (float)CharX / (float)font->Width;
        v[vertSh].texcoord[1] = (float)CharY / (float)font->Height;
        v[vertSh].position[0] = (float)curX + OffsetX;
        v[vertSh++].position[1] = (float)OffsetY;

        //upper right
        v[vertSh].texcoord[0] = (float)(CharX + Width) / (float)font->Width;
        v[vertSh].texcoord[1] = (float)CharY / (float)font->Height;
        v[vertSh].position[0] = (float)Width + curX + OffsetX;
        v[vertSh++].position[1] = (float)OffsetY;

        //lower right
        v[vertSh].texcoord[0] = (float)(CharX + Width) / (float)font->Width;
        v[vertSh].texcoord[1] = (float)(CharY + Height) / (float)font->Height;
        v[vertSh].position[0] = (float)Width + curX + OffsetX;
        v[vertSh++].position[1] = (float)Height + OffsetY;

        //lower left
        v[vertSh].texcoord[0] = (float)CharX / (float)font->Width;
        v[vertSh].texcoord[1] = (float)(CharY + Height) / (float)font->Height;
        v[vertSh].position[0] = (float)curX + OffsetX;
        v[vertSh++].position[1] = (float)Height + OffsetY;

        curX += cp->XAdvance;
    }

    auto* pCtxMgr = GetContextManager();
    mesh = pCtxMgr->GetMeshManager()->CreateMeshFromVertices(v, i, {});
}

ITexturePtr TextSceneNode::GetTexture() const
{
    return font->GetTexturePtr();
}
