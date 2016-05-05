#include "VertexTypes.h"

#include <cassert>

using namespace ShiftEngine;

VertexSemantic ShiftEngine::defaultVertexSemantic;
VertexSemantic ShiftEngine::extendedVertexSemantic;
VertexSemantic ShiftEngine::colorVertexSemantic;
VertexSemantic ShiftEngine::plainSpriteVertexSemantic;

VertexInfo::VertexInfo()
    : type(ET_FLOAT)
    , count(1)
    , semantic(ES_Custom)
{}

VertexInfo::VertexInfo(ElemType _type, unsigned _count, ElemSemantic _sem, const std::string & /*name*/ /*= ""*/)
    : type(_type)
    , count(_count)
    , semantic(_sem)
{}

bool VertexInfo::operator!=(const VertexInfo & ref) const
{
    return type != ref.type
        || count != ref.count
        || semantic != ref.semantic
        || name != ref.name;
}

bool VertexInfo::operator<(const VertexInfo & ref) const
{
    if (type != ref.type) return type < ref.type;
    if (count != ref.count) return count < ref.count;
    if (semantic != ref.semantic) return semantic < ref.semantic;
    if (name != ref.name) return name < ref.name;
    return false;
}

void VertexSemantic::addSemantic(ElemType type, unsigned count, ElemSemantic semantic, const std::string & name /*= ""*/)
{
    vertexSemantics.push_back(VertexInfo(type, count, semantic, name));
}

const std::vector<VertexInfo> & VertexSemantic::getInnerRepresentation() const
{
    return vertexSemantics;
}

bool VertexSemantic::operator<(const VertexSemantic & ref) const
{
    if (vertexSemantics.size() != ref.vertexSemantics.size())
        return vertexSemantics.size() < ref.vertexSemantics.size();

    for (size_t i = 0; i < vertexSemantics.size(); ++i)
        if (vertexSemantics[i] != ref.vertexSemantics[i])
            return vertexSemantics[i] < ref.vertexSemantics[i];

    return false;
}

bool VertexSemantic::isNormalsHere() const
{
    for (auto elem : vertexSemantics)
        if (elem.semantic == ES_Normal)
            return true;

    return false;
}

bool VertexSemantic::isTexcoordsHere() const
{
    for (auto elem : vertexSemantics)
        if (elem.semantic == ES_Texcoord)
            return true;

    return false;
}

bool VertexSemantic::isVertexColorsHere() const
{
    for (auto elem : vertexSemantics)
        if (elem.semantic == ES_Color)
            return true;

    return false;
}

bool VertexSemantic::isTangentsHere() const
{
    for (auto elem : vertexSemantics)
        if (elem.semantic == ES_Tangent)
            return true;

    return false;
}

bool VertexSemantic::isBinormalsHere() const
{
    for (auto elem : vertexSemantics)
        if (elem.semantic == ES_Binormal)
            return true;

    return false;
}

size_t VertexSemantic::getVertexSize() const
{
    size_t vertexSize = 0;
    for (const auto & elem : vertexSemantics)
    {
        switch (elem.type)
        {
        case ET_FLOAT:
            vertexSize += elem.count * sizeof(float);
            break;
        default:
            assert(false);
            break;
        }
    }

    return vertexSize;
}
