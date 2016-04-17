#pragma once

#define LIM_HEADER_VERSION 3

#include <MathLib/math.h>

#include <cstdint>

struct MeshLIMHeader
{
    const uint32_t version = LIM_HEADER_VERSION;
    bool hasNormals = false;
    bool hasTexCoords = false;
    bool hasColors = false;
    uint32_t verticesCount = 0;
    uint32_t indicesCount = 0;
};

struct Vertex
{
    Vertex(const MathLib::vec3f &Pos = {},
           const MathLib::vec3f &Normal = {},
           const MathLib::vec2f &TexCoord = {},
           const MathLib::vec3f &Color = {})
        : Pos(Pos)
        , Normal(Normal)
        , TexCoord(TexCoord)
        , Color(Color)
    {
    }

    MathLib::vec3f Pos;
    MathLib::vec3f Normal;
    MathLib::vec2f TexCoord;
    MathLib::vec3f Color;
};
