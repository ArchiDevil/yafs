#include "Material.h"

#include <cassert>

using namespace ShiftEngine;

Material::Material(IProgramPtr _program)
    : program(_program)
{
    for (int i = 0; i < SV_Elems; i++)
        builtinVarIndices[i] = 0;

    for (int i = 0; i < ST_Elems; i++)
        builtinTextures[i] = std::make_pair(UINT_MAX, nullptr);

    if (_program)
        LinkMaterial();
}

Material::Material(MaterialInfo & mtlParams)
    : info(mtlParams)
{
    for (int i = 0; i < SV_Elems; i++)
        builtinVarIndices[i] = 0;

    for (int i = 0; i < ST_Elems; i++)
        builtinTextures[i] = std::make_pair(UINT_MAX, nullptr);
}

Material::Material(IProgramPtr _program, MaterialInfo & mtlParams)
    : program(_program)
    , info(mtlParams)
{
    for (int i = 0; i < SV_Elems; i++)
        builtinVarIndices[i] = 0;

    for (int i = 0; i < ST_Elems; i++)
        builtinTextures[i] = std::make_pair(UINT_MAX, nullptr);

    if (_program)
        LinkMaterial();
}

void Material::LinkMaterial()
{
    program->GetVariableIndex("matWorld", &builtinVarIndices[SV_MatWorld]);
    program->GetVariableIndex("matView", &builtinVarIndices[SV_MatView]);
    program->GetVariableIndex("matProjection", &builtinVarIndices[SV_MatProj]);

    program->GetVariableIndex("eyePos", &builtinVarIndices[SV_EyePos]);
    program->GetVariableIndex("ambientColor", &builtinVarIndices[SV_AmbientColor]);

    program->GetVariableIndex("lights", &builtinVarIndices[SV_LightsArray]);
    program->GetVariableIndex("lightsCount", &builtinVarIndices[SV_LightsCount]);

    program->GetVariableIndex("diffuseColor", &builtinVarIndices[SV_DiffuseColor]);
    program->GetVariableIndex("specularColor", &builtinVarIndices[SV_SpecularColor]);
    program->GetVariableIndex("emissionColor", &builtinVarIndices[SV_EmissionColor]);
    program->GetVariableIndex("opacity", &builtinVarIndices[SV_Transparency]);
    program->GetVariableIndex("shininess", &builtinVarIndices[SV_Shininess]);

    for (int i = 0; i < SV_Elems; i++)
    {
        if (builtinVarIndices[i] != (unsigned)-1)
        {
            knownUniforms.push_back((engineUniforms)i);
        }
    }

    program->GetResourceIndex("diffuseMap", &builtinTextures[ST_Diffuse].first);
    program->GetResourceIndex("alphaMap", &builtinTextures[ST_Alpha].first);
}

// void cMaterial::SetCustomTexture( cTexturePtr texture, const char * name )
// {
// 	textures[name] = texture;
// }

// void cMaterial::UnbindAllTextures()
// {
// 	for(auto & elem : builtinTexIndices)
// 		elem = UINT_MAX;
// }

void Material::SetDiffuseTexture(const ITexturePtr & texture)
{
    if (builtinTextures[ST_Diffuse].first != UINT_MAX)
    {
        builtinTextures[ST_Diffuse].second = texture;
    }
    else
    {
        builtinTextures[ST_Diffuse].second = texture;
        program = nullptr;
        info.diffuseMap = MaterialTextureDescription(texture->GetType());
    }
}

void Material::SetAlphaTexture(const ITexturePtr & texture)
{
    if (builtinTextures[ST_Alpha].first != UINT_MAX)
    {
        builtinTextures[ST_Alpha].second = texture;
    }
    else
    {
        builtinTextures[ST_Alpha].second = texture;
        program = nullptr;
        info.alphaMap = MaterialTextureDescription(texture->GetType());
    }
}

void Material::SetNamedParam(const std::string & parameter, const float param)
{
    unsigned int var = 0;
    if (program)
        program->GetVariableIndex(parameter.c_str(), &var);
    floatParams[parameter] = param;
}

void Material::SetNamedParam(const std::string & parameter, const MathLib::vec2f & param)
{
    unsigned int var = 0;
    if (program)
        program->GetVariableIndex(parameter.c_str(), &var);
    float2Params[parameter] = param;
}

void Material::SetNamedParam(const std::string & parameter, const MathLib::vec3f & param)
{
    unsigned int var = 0;
    if (program)
        program->GetVariableIndex(parameter.c_str(), &var);
    float3Params[parameter] = param;
}

void Material::SetNamedParam(const std::string & parameter, const MathLib::vec4f & param)
{
    unsigned int var = 0;
    if (program)
        program->GetVariableIndex(parameter.c_str(), &var);
    float4Params[parameter] = param;
}

void Material::SetZState(bool zState)
{
    ZState = zState;
}

//must be equal to shader struct with 4-byte pack!
struct shaderLightRefl
{
    MathLib::vec3f color = {};
    float type = 0.0f;
    MathLib::vec3f dir = {};
    float radius = 0.0f;
    MathLib::vec3f pos = {};
    float pack = 0.0f;
};

void Material::BindLights(const std::vector<LightInfo> & lights)
{
    if (!program || lights.size() > 4)
        return;

    shaderLightRefl binder[4];
    for (unsigned int i = 0; i < lights.size(); i++)
    {
        binder[i].color = lights[i].Color;

        if (lights[i].Type == LNT_Directional)
        {
            binder[i].type = 0.5f;
            binder[i].dir = lights[i].Direction;
        }
        else if (lights[i].Type == LNT_Point)
        {
            binder[i].type = 1.5f;
            binder[i].pos = lights[i].Position;
            binder[i].radius = lights[i].Radius;
        }
    }

    return program->SetArrayConstantByIndex(builtinVarIndices[SV_LightsArray], binder);
}

MaterialInfo * Material::GetMaterialInfo()
{
    return &info;
}

unsigned int Material::GetUniformIndex(engineUniforms var) const
{
    return builtinVarIndices[var];
}

const std::vector<engineUniforms> & Material::GetUniforms() const
{
    return knownUniforms;
}

void Material::SetDiffuseColor(const MathLib::vec4f & diffuseColor)
{
    info.diffuseColor = diffuseColor;
}

void Material::SetSpecularColor(const MathLib::vec4f & specularColor)
{
    info.specularColor = specularColor;
}

void Material::SetEmissionColor(const MathLib::vec4f & emissionColor)
{
    info.emissionColor = emissionColor;
}
