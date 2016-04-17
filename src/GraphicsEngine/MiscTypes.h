#pragma once

#include <string>
#include <cstdint>

#include <MathLib/math.h>

#define LIM_HEADER_VERSION 3

namespace ShiftEngine
{
    //������� ������-�������� ��� ������
    enum RTFormats
    {
        Format_RGBA8,
        Format_R32,
    };

    enum class TextureType : uint8_t
    {
        Unknown,
        Texture2D,
        Texture2DArray,
        TextureCubemap,
        Texture3D
    };

    //����� ��� ����� � ��������
    struct MeshLIMHeader
    {
        MeshLIMHeader(int VERSION = 0,
            bool hasNormals = false,
            bool hasTexCoords = false,
            bool hasColors = false,
            unsigned int verticesCount = 0,
            unsigned int indicesCount = 0)
            : VERSION(VERSION)
            , hasNormals(hasNormals)
            , hasTexCoords(hasTexCoords)
            , hasColors(hasColors)
            , verticesCount(verticesCount)
            , indicesCount(indicesCount)
        {
        }

        unsigned int VERSION;
        bool hasNormals;
        bool hasTexCoords;
        bool hasColors;
        unsigned int verticesCount;
        unsigned int indicesCount;
    };

    //������������� �������
    struct gRect
    {
        float left;
        float top;
        float right;
        float bottom;
    };

    //��������� ������������ ������
    struct GraphicEngineSettings
    {
        GraphicEngineSettings()
            : screenWidth(800)
            , screenHeight(600)
            , multisampleQuality(0)
            , windowed(true)
            , screenRate(0)
            , zNear(0.1f)
            , zFar(200.0f)
            , anisotropyLevel(0)
        {
        }

        int screenWidth;
        int screenHeight;
        int multisampleQuality;
        bool windowed;
        int screenRate;
        float zNear;
        float zFar;
        int anisotropyLevel;
    };

    //��������� ������ ������������� ���� ��� �������� ��������
    struct PathSettings
    {
        std::wstring MeshPath;
        std::wstring TexturePath;
        std::wstring ShaderPath;
        std::wstring FontsPath;
        std::wstring MaterialsPath;
    };

} // end of ShiftEngine namespace
