#include "FontManager.h"

#include "ShiftEngine.h"

#include <Utilities/ut.h>
#include <Utilities/logger.hpp>

#include <cassert>

using namespace ShiftEngine;

FontManager::FontManager()
    : currentFont(L"")
{
    LoadFonts();
    if (!Fonts.empty())
        currentFont = Fonts.begin()->first;
}

void FontManager::DrawTextTL(const std::string & Text, float x, float y)
{
    if (!pCurrentFont || Text.empty())
        return;

    BatchText(Text, x, y);
}

int FontManager::GetFontHeight()
{
    if (!pCurrentFont)
        return 0;
    else
        return pCurrentFont->LineHeight;
}

int FontManager::GetStringWidth(const std::string & string)
{
    int result = 0;

    if (!pCurrentFont)
        return 0;

    for (unsigned char symbol : string)
    {
        auto cp = pCurrentFont->GetCharacterPtr(symbol);
        if (!cp->Height)
            return 0;

        result += cp->XAdvance;
    }

    return result;
}

void FontManager::LoadFonts()
{
    IContextManager* pCntMng = GetContextManager();
    //TODO: добавить механизм, чтобы файл шрифта содержал имя текстуры
    PathSettings pPaths = pCntMng->GetPaths();
    auto fontsNames = utils::filesystem::collect_file_names(pPaths.FontsPath, L"fnt2");
    auto fontsTextureNames = utils::filesystem::collect_file_names(pPaths.TexturePath + L"fonts\\");

    if (fontsNames.empty() || fontsTextureNames.empty() || fontsNames.size() != fontsTextureNames.size())
        LOG_FATAL_ERROR("Unable to load textures for fonts");

    auto fontNamesIterator = fontsNames.begin();
    auto textureNamesIterator = fontsTextureNames.begin();

    while (fontNamesIterator != fontsNames.end())
    {
        cFont * font = nullptr;
        try
        {
            font = new cFont(pPaths.FontsPath + *fontNamesIterator, pCntMng->LoadTexture(L"fonts\\" + *textureNamesIterator));
        }
        catch (const std::exception&)
        {
            LOG_ERROR("Unable to load font: ", utils::narrow(*fontNamesIterator));
        }
        Fonts[::utils::extract_name(*fontNamesIterator)].reset(font);
        ++textureNamesIterator;
        ++fontNamesIterator;
    }
}

void FontManager::SetFont(const std::wstring & fontName)
{
    auto iter = Fonts.find(fontName);
    if (iter != Fonts.end())
    {
        currentFont = fontName;
        pCurrentFont = iter->second.get();
    }
    else
    {
        pCurrentFont = nullptr;
        currentFont.clear();
        LOG_ERROR("Unable to find font: ", utils::narrow(fontName));
    }
}

std::wstring FontManager::GetCurrentFontName() const
{
    return currentFont;
}

void FontManager::BatchText(const std::string & text, float x, float y)
{
    if (text.empty())
        return;

    auto & v = batchedVertices[GetCurrentFontName()];
    auto & i = batchedIndices[GetCurrentFontName()];

    size_t vertSh = v.size();
    size_t indSh = i.size();

    i.resize(i.size() + text.size() * 6);
    v.resize(v.size() + text.size() * 4);

    int CurX = 0;

    for (char symbol : text)
    {
        sChar * cp = pCurrentFont->GetCharacterPtr(symbol);
        assert(cp->Height != 0);
        int CharX = cp->x;
        int CharY = cp->y;
        int Width = cp->Width;
        int Height = cp->Height;
        int OffsetX = cp->XOffset;
        int OffsetY = cp->YOffset;

        //first triangle
        i[indSh++] = vertSh;
        i[indSh++] = vertSh + 1;
        i[indSh++] = vertSh + 2;

        //second triangle
        i[indSh++] = vertSh;
        i[indSh++] = vertSh + 2;
        i[indSh++] = vertSh + 3;

        /*
        0   1
        -----
        |\  |
        | \ |
        |  \|
        -----
        3   2
        */

        //upper left
        v[vertSh].texcoord[0] = (float)CharX / (float)pCurrentFont->Width;
        v[vertSh].texcoord[1] = (float)CharY / (float)pCurrentFont->Height;
        v[vertSh].position[0] = (float)CurX + OffsetX + x;
        v[vertSh++].position[1] = (float)OffsetY + y;

        //upper right
        v[vertSh].texcoord[0] = (float)(CharX + Width) / (float)pCurrentFont->Width;
        v[vertSh].texcoord[1] = (float)CharY / (float)pCurrentFont->Height;
        v[vertSh].position[0] = (float)Width + CurX + OffsetX + x;
        v[vertSh++].position[1] = (float)OffsetY + y;

        //lower right
        v[vertSh].texcoord[0] = (float)(CharX + Width) / (float)pCurrentFont->Width;
        v[vertSh].texcoord[1] = (float)(CharY + Height) / (float)pCurrentFont->Height;
        v[vertSh].position[0] = (float)Width + CurX + OffsetX + x;
        v[vertSh++].position[1] = (float)Height + OffsetY + y;

        //lower left
        v[vertSh].texcoord[0] = (float)CharX / (float)pCurrentFont->Width;
        v[vertSh].texcoord[1] = (float)(CharY + Height) / (float)pCurrentFont->Height;
        v[vertSh].position[0] = (float)CurX + OffsetX + x;
        v[vertSh++].position[1] = (float)Height + OffsetY + y;

        CurX += cp->XAdvance;
    }
}

void FontManager::DrawBatchedText()
{
    for (auto & p : batchedVertices)
    {
        //auto & v = batchedVertices[p.first];
        //auto & i = batchedIndices[p.first];

        //if (v.empty() || i.empty())
        //    return;

        //auto* pCtxMgr = GetContextManager();

        //batchedMesh = pCtxMgr->GetMeshManager()->CreateMeshFromVertices(v, i, {});

        //GraphicEngineSettings pSettings = pCtxMgr->GetEngineSettings();
        //MathLib::mat4f matOrtho = MathLib::matrixOrthoOffCenterLH<float>(0.0f, (float)pSettings.screenWidth, (float)pSettings.screenHeight, 0.0f, 0.0f, 1.0f);
        //textShader->SetMatrixConstantByName("matOrtho", matOrtho);
        //textShader->SetTextureByName("FontTexture", pCurrentFont->GetTexturePtr());
        //textShader->Apply(true);

        //batchedMesh->Draw();

        //v.clear();
        //i.clear();
        //batchedMesh = nullptr;
    }
}
