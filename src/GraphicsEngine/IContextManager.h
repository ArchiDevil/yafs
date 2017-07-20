#pragma once

#include "FontManager.h"
#include "MiscTypes.h"
#include "ITextureManager.h"
#include "IMeshManager.h"
#include "RenderQueue.h"

namespace ShiftEngine
{
class IContextManager
{
public:
    virtual ~IContextManager() = default;

    virtual ITexturePtr                     LoadTexture(const std::wstring & FileName) = 0;
    virtual ITextureManager *               GetTextureManager() = 0;
    virtual IMeshManager *                  GetMeshManager() = 0;
    virtual FontManager*                    GetFontManager() = 0;
    virtual const GraphicEngineSettings &   GetEngineSettings() const = 0;
    virtual const PathSettings &            GetPaths() const = 0;
    virtual void                            DrawAll(RenderQueue& queue, double dt) = 0;
    virtual void                            SetWireframeState(bool state) = 0;
};
}
