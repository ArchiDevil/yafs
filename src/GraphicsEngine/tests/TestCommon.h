#pragma once

#include <GraphicsEngine/ShiftEngine.h>

using namespace ShiftEngine;

template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<CameraFrustum::CullingStatus>(const CameraFrustum::CullingStatus& t)
{
    std::wostringstream s;
    s << (int)t << "\n";
    return s.str();
}
