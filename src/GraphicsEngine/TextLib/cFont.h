#pragma once

/*
    We are using BMFont
*/

#include <string>
#include <fstream>
#include <sstream>
#include <cstdint>

#include "../ITexture.h"

namespace ShiftEngine
{

struct sChar
{
    //this code taken from tutorial about BMFont
    //clean 16 bytes
    uint16_t x = 0, y = 0;
    uint16_t Width = 0, Height = 0;
    int16_t XOffset = 0;
    uint16_t YOffset = 0;
    uint16_t XAdvance = 0;
    uint16_t Page = 0;
};

class cFont
{
public:
    cFont(const std::wstring& filename, ShiftEngine::ITexturePtr tp)
        : filename(filename)
        , tp(tp)
    {
        std::ifstream in;
        in.open(filename);
        if (!in || in.fail())
            throw std::runtime_error("Unable to find font file");

        std::string Line;
        std::string Read, Key, Value;
        std::size_t i;

        while (!in.eof())
        {
            std::stringstream LineStream;
            std::getline(in, Line);
            LineStream << Line;

            //read the line's type
            LineStream >> Read;
            if (Read == "common")
            {
                //this holds common data
                while (!LineStream.eof())
                {
                    std::stringstream Converter;
                    LineStream >> Read;
                    i = Read.find('=');
                    Key = Read.substr(0, i);
                    Value = Read.substr(i + 1);

                    //assign the correct value
                    Converter << Value;
                    if (Key == "lineHeight")
                        Converter >> LineHeight;
                    else if (Key == "base")
                        Converter >> Base;
                    else if (Key == "scaleW")
                        Converter >> Width;
                    else if (Key == "scaleH")
                        Converter >> Height;
                    else if (Key == "pages")
                        Converter >> Pages;
                }
            }
            else if (Read == "char")
            {
                //this is data for a specific char
                unsigned short CharID = 0;

                while (!LineStream.eof())
                {
                    std::stringstream Converter;
                    LineStream >> Read;
                    i = Read.find('=');
                    Key = Read.substr(0, i);
                    Value = Read.substr(i + 1);

                    //assign the correct value
                    Converter << Value;
                    if (Key == "id")
                        Converter >> CharID;
                    else if (Key == "x")
                        Converter >> data[CharID].x;
                    else if (Key == "y")
                        Converter >> data[CharID].y;
                    else if (Key == "width")
                        Converter >> data[CharID].Width;
                    else if (Key == "height")
                        Converter >> data[CharID].Height;
                    else if (Key == "xoffset")
                        Converter >> data[CharID].XOffset;
                    else if (Key == "yoffset")
                        Converter >> data[CharID].YOffset;
                    else if (Key == "xadvance")
                        Converter >> data[CharID].XAdvance;
                    else if (Key == "page")
                        Converter >> data[CharID].Page;
                }
            }
        }
    }

    inline sChar * GetCharacterPtr(unsigned char code)
    {
        return &data[code];
    }

    ShiftEngine::ITexturePtr GetTexturePtr()
    {
        return tp;
    }

    uint16_t LineHeight = 0;
    uint16_t Base = 0;
    uint16_t Width = 0;
    uint16_t Height = 0;

private:
    ShiftEngine::ITexturePtr tp = nullptr;
    std::wstring filename = L"";

    uint16_t Pages = 0;
    sChar data[256];    //for non-unicode characters

};

}