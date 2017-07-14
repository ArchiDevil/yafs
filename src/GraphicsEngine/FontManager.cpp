#include "FontManager.h"

#include "ShiftEngine.h"

#include <Utilities/ut.h>
#include <Utilities/logger.hpp>

using namespace ShiftEngine;

FontManager::FontManager(IContextManager* contextManager)
    : contextManager(contextManager)
{
    LoadFonts();
}

void FontManager::LoadFonts()
{
    //TODO: добавить механизм, чтобы файл шрифта содержал имя текстуры
    PathSettings pPaths = contextManager->GetPaths();
    auto fontsNames = utils::filesystem::collect_file_names(pPaths.FontsPath, L"fnt2");
    auto fontsTextureNames = utils::filesystem::collect_file_names(pPaths.TexturePath + L"fonts/");

    if (fontsNames.empty() || fontsTextureNames.empty() || fontsNames.size() != fontsTextureNames.size())
        LOG_FATAL_ERROR("Unable to load textures for fonts");

    auto fontNamesIterator = fontsNames.begin();
    auto textureNamesIterator = fontsTextureNames.begin();

    while (fontNamesIterator != fontsNames.end())
    {
        cFont* font = nullptr;
        try
        {
            font = new cFont(pPaths.FontsPath + *fontNamesIterator, contextManager->LoadTexture(L"fonts/" + *textureNamesIterator));
        }
        catch (const std::exception&)
        {
            LOG_ERROR("Unable to load font: ", utils::narrow(*fontNamesIterator));
        }
        fonts[::utils::extract_name(*fontNamesIterator)].reset(font);
        ++textureNamesIterator;
        ++fontNamesIterator;
    }
}

TextSceneNodePtr FontManager::CreateTextSceneNode(const std::string& text, MathLib::vec2f position, const std::wstring& fontName)
{
    textSceneNodes.emplace_back(std::make_shared<TextSceneNode>(text, position, fonts.at(fontName).get()));
    return textSceneNodes.back();
}

void FontManager::DrawAll(RenderQueue& queue) const
{
    for (auto& node : textSceneNodes)
        queue.AddTextNode(node.get());
}
