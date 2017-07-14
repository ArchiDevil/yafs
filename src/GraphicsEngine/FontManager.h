#pragma once

#include "TextLib/cFont.h"
#include <IMeshData.h>

#include <Utilities/IManager.h>

#include <map>
#include <vector>

namespace ShiftEngine
{

class FontManager : public IManager
{
public:
    FontManager();

    void DrawTextTL(const std::string & Text, float x, float y);

    int GetFontHeight();
    int GetStringWidth(const std::string & string);

    void SetFont(const std::wstring & fontName);
    std::wstring GetCurrentFontName() const;

    void DrawBatchedText();

private:
    void LoadFonts();
    void BatchText(const std::string & text, float x, float y);

    std::map<std::wstring, std::vector<SpriteVertex>> batchedVertices;
    std::map<std::wstring, std::vector<uint32_t>> batchedIndices;
    IMeshDataPtr batchedMesh = nullptr;

    std::map<std::wstring, std::unique_ptr<cFont>> Fonts;
    cFont * pCurrentFont = nullptr;
    std::wstring currentFont = L"";
};

}
