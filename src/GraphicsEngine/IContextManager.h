#pragma once

#include "FontManager.h"
#include "MiscTypes.h"
#include "ITextureManager.h"
#include "RenderQueue.h"

namespace ShiftEngine
{
class IContextManager
{
public:
    virtual ~IContextManager() {}

    virtual bool                            Initialize(GraphicEngineSettings settings, PathSettings paths) = 0;
    virtual std::wstring                    GetGPUDescription() = 0;
    virtual ITexturePtr                     LoadTexture(const std::wstring & FileName) = 0;
    virtual ITextureManager *               GetTextureManager() = 0;
    // virtual FontManager*                    GetFontManager() = 0;
    virtual const GraphicEngineSettings &   GetEngineSettings() const = 0;
    virtual const PathSettings &            GetPaths() const = 0;
    virtual void                            DrawAll(RenderQueue& queue, double dt) = 0;
};
}
