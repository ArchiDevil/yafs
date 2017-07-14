#pragma once

#include "TextLib/cFont.h"
#include "TextLib/TextSceneNode.h"

#include <IMeshData.h>
#include <RenderQueue.h>

#include <Utilities/IManager.h>

#include <map>
#include <vector>
#include <string>

namespace ShiftEngine
{

class IContextManager;

class FontManager : public IManager
{
public:
    FontManager(IContextManager* contextManager);

    TextSceneNodePtr    CreateTextSceneNode(const std::string& text, MathLib::vec2f position, const std::wstring& fontName);
    void                DrawAll(RenderQueue& queue) const;

private:
    void                LoadFonts();

    IContextManager*                                contextManager = nullptr;
    std::vector<TextSceneNodePtr>                   textSceneNodes;
    std::map<std::wstring, std::unique_ptr<cFont>>  fonts;

};

}
