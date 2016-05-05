#include "MaterialInfo.h"

using namespace ShiftEngine;

materialFlags::materialFlags()
    : useVertexColors(false)
    , isCustom(false)
    , isReflective(false)
    , isTransparent(false)
    , twoFace(false)
    , lightingModel(MaterialLightingModel::Phong)
    , useAnimation(false)
{
}

bool materialFlags::operator==(const materialFlags & ref) const
{
    return useVertexColors == ref.useVertexColors
        && lightingModel == ref.lightingModel
        && twoFace == ref.twoFace
        && isReflective == ref.isReflective
        && isTransparent == ref.isTransparent
        && isCustom == ref.isCustom;
}

uint32_t materialFlags::GetHash() const
{
    static_assert(sizeof(lightingModel) == sizeof(uint8_t), "Update hash calculating");
    uint32_t out = 0x0;
    out |= useVertexColors << 0;
    out |= twoFace << 1;
    out |= isReflective << 2;
    out |= isTransparent << 3;
    out |= isCustom << 4;
    out |= (uint32_t)lightingModel << 5;
    return out;
}

//////////////////////////////////////////////////////////////////////////

MaterialInfo::MaterialInfo()
    : diffuseColor(1.0f, 1.0f, 1.0f, 1.0f)
    , specularColor(1.0f, 1.0f, 1.0f, 1.0f)
    , emissionColor(0.0f, 0.0f, 0.0f, 0.0f)
    , opacity(1.0f)
    , shininess(25.0f)
{
}

MaterialInfo::MaterialInfo(const materialFlags & _flags)
    : diffuseColor(1.0f, 1.0f, 1.0f, 1.0f)
    , specularColor(1.0f, 1.0f, 1.0f, 1.0f)
    , emissionColor(0.0f, 0.0f, 0.0f, 0.0f)
    , opacity(1.0f)
    , shininess(25.0f)
    , flags(_flags)
{

}

const materialFlags * MaterialInfo::GetFlags() const
{
    return &flags;
}

bool MaterialInfo::operator<(const MaterialInfo & ref) const
{
    if (*this == ref)
        return false;

    return this->GetHash() < ref.GetHash();
}

bool MaterialInfo::operator==(const MaterialInfo & ref) const
{
    return flags == ref.flags
        && diffuseColor == ref.diffuseColor
        && specularColor == ref.specularColor
        && emissionColor == ref.emissionColor
        && diffuseMap == ref.diffuseMap
        //&& normalMap == ref.normalMap
        //&& specularMap == ref.specularMap
        //&& environmentMap == ref.environmentMap
        && alphaMap == ref.alphaMap
        && opacity == ref.opacity
        && shininess == ref.shininess;
}

uint64_t MaterialInfo::GetHash() const
{
    uint64_t out = 0x0;
    // serialize flags first
    out |= flags.GetHash();

    // next - textures
    out |= (uint64_t)diffuseMap.GetType() << 32;
    out |= (uint64_t)alphaMap.GetType() << 40;
    return out;
}

//////////////////////////////////////////////////////////////////////////

MaterialTextureDescription::MaterialTextureDescription(TextureType _type)
    : type(_type)
{
}

TextureType MaterialTextureDescription::GetType() const
{
    return type;
}

const std::vector<std::string> & MaterialTextureDescription::GetMaps() const
{
    return maps;
}

bool MaterialTextureDescription::operator==(const MaterialTextureDescription & ref) const
{
    if (this->type != ref.type)
        return false;

    if (this->maps.size() != ref.maps.size())
        return false;

    for (unsigned int i = 0; i < maps.size(); i++)
        if (this->maps[i] != ref.maps[i])
            return false;

    return true;
}

void MaterialTextureDescription::SetMaps(const std::vector<std::string> & maps)
{
    this->maps = maps;
}
