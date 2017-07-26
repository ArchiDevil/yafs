#pragma once

#include <string>
#include <cstdint>

#define LIM_HEADER_VERSION 3

namespace ShiftEngine
{
// Render target formats for the engine
enum class RTFormats : uint8_t
{
    Format_RGBA8,
    Format_R32,
};

// Different texture types
enum class TextureType : uint8_t
{
    Unknown,
    Texture2D,
    Texture2DArray,
    TextureCubemap,
    Texture3D
};

enum class SpriteFilterMode
{
    Point,
    Linear,
    Anisotropy
};

// Mesh file header
struct MeshLIMHeader
{
    unsigned int VERSION = 0;
    bool hasNormals = false;
    bool hasTexCoords = false;
    bool hasColors = false;
    unsigned int verticesCount = 0;
    unsigned int indicesCount = 0;
};

// Rectangle
struct gRect
{
    float left;
    float top;
    float right;
    float bottom;
};

// Different settings for graphics engine
struct GraphicEngineSettings
{
    GraphicEngineSettings() = default;

    int screenWidth = 800;
    int screenHeight = 600;
    int multisampleQuality = 0;
    bool windowed = true;
    int screenRate = 0;
    int anisotropyLevel = 0;
    SpriteFilterMode spriteFiltering = SpriteFilterMode::Point;
};

// Relative paths to the resources
struct PathSettings
{
    std::wstring TexturePath;
    std::wstring ShaderPath;
    std::wstring FontsPath;
};

} // end of ShiftEngine namespace
