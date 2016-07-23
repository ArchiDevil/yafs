#include "D3D11TextureManager.h"

#include <Utilities/ut.h>
#include <Utilities/logger.hpp>

#include <DirectXTex.h>

#include <array>

using namespace ShiftEngine;

D3D11TextureManager::D3D11TextureManager(CComPtr<ID3D11Device> device, CComPtr<ID3D11DeviceContext> pDeviceContext, const std::wstring & texturesPath)
    : texturesPath(texturesPath)
    , pDevice(device)
    , pDeviceContext(pDeviceContext)
{
    CreateErrorTexture();
}

ITexturePtr D3D11TextureManager::CreateTexture2D(const std::wstring & FileName)
{
    std::wstring innerName = texturesPath + FileName;
    auto iter = textures.find(innerName);

    if (iter != textures.end() && iter->second)
    {
        return D3D11TexturePtr(iter->second);
    }
    else
    {
        CComPtr<ID3D11ShaderResourceView> shaderResource = nullptr;
        CComPtr<ID3D11Texture2D> textureResource = nullptr;
        DirectX::TexMetadata metadata;
        DirectX::ScratchImage image;

        HRESULT hr = DirectX::LoadFromWICFile(innerName.c_str(), 0, &metadata, image);

        if (FAILED(hr))
        {
            LOG_ERROR("Unable to load file: ", utils::narrow(FileName), ", error code: ", hr);
            return GetErrorTexture();
        }

        hr = DirectX::CreateTextureEx(pDevice,
                                      image.GetImage(0, 0, 0),
                                      1,
                                      metadata,
                                      D3D11_USAGE_DEFAULT,
                                      D3D11_BIND_SHADER_RESOURCE,
                                      0,
                                      0,
                                      true,
                                      (ID3D11Resource**)&textureResource);

        if (FAILED(hr))
        {
            LOG_ERROR("Unable to create texture from file: ", utils::narrow(FileName), ", error code: ", hr);
            return GetErrorTexture();
        }
        
        // getting sRGB format from loaded texture
        D3D11_TEXTURE2D_DESC desc = {};
        textureResource->GetDesc(&desc);

        D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
        viewDesc.Format = desc.Format;
        viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MostDetailedMip = 0;
        viewDesc.Texture2D.MipLevels = desc.MipLevels;

        hr = pDevice->CreateShaderResourceView(textureResource, &viewDesc, &shaderResource);

        if (FAILED(hr))
        {
            LOG_ERROR("Unable to create ShaderResourceView, error code: ", hr);
            return GetErrorTexture();
        }

        D3D11TexturePtr buffer = std::make_shared<D3D11Texture>(pDeviceContext, metadata.width, metadata.height, TextureType::Texture2D, shaderResource);
        textures[innerName] = buffer;
        return buffer;
    }
}

ITexturePtr ShiftEngine::D3D11TextureManager::CreateCubemap(const std::wstring & posX,
                                                            const std::wstring & negX,
                                                            const std::wstring & posY,
                                                            const std::wstring & negY,
                                                            const std::wstring & posZ,
                                                            const std::wstring & negZ)
{
    std::wstring superString = negX + posX + negY + posY + negZ + posZ;

    if (negX.empty() || posX.empty() ||
        negY.empty() || posY.empty() ||
        negZ.empty() || posZ.empty())
    {
        LOG_ERROR("Unable to load cubemap with empty textures");
        return nullptr;
    }

    static const size_t elemsCount = 6;
    std::array<std::wstring, elemsCount> textureNames = {posX, negX, posY, negY, posZ, negZ};

    std::vector<DirectX::TexMetadata> metadatas;
    for (size_t i = 0; i < elemsCount; ++i)
    {
        DirectX::TexMetadata ii;

        HRESULT hr = DirectX::GetMetadataFromWICFile((texturesPath + textureNames[i]).c_str(), 0, ii);

        if (FAILED(hr))
            LOG_ERROR("Unable to get image info for: ", utils::narrow(textureNames[i]), ", error code: ", hr);

        metadatas.push_back(ii);
    }

    if (metadatas.size() > 1)
    {
        size_t prevWidth = metadatas[0].width;
        size_t prevHeight = metadatas[0].height;
        for (size_t i = 1; i < metadatas.size(); ++i)
        {
            if (metadatas[i].width != prevWidth || metadatas[i].height != prevHeight)
            {
                LOG_FATAL_ERROR("Unable to create cubemap from textures without the same size");
                return nullptr;
            }
        }
    }

    std::array<CComPtr<ID3D11Texture2D>, elemsCount> srcTex = {};

    for (size_t i = 0; i < elemsCount; ++i)
    {
        bool error = false;
        DirectX::ScratchImage image;
        HRESULT hr = DirectX::LoadFromWICFile((texturesPath + textureNames[i]).c_str(), 0, &metadatas[i], image);

        if (FAILED(hr))
        {
            error = true;
            LOG_ERROR("Unable to load file: ", utils::narrow(textureNames[i]), ", error code: ", hr);
        }

        hr = DirectX::CreateTextureEx(pDevice, 
                                      image.GetImage(0, 0, 0), 
                                      1, 
                                      metadatas[i], 
                                      D3D11_USAGE_DEFAULT,
                                      D3D11_BIND_SHADER_RESOURCE,
                                      0,
                                      0,
                                      true, 
                                      (ID3D11Resource**)&srcTex[i]);

        if (FAILED(hr))
        {
            error = true;
            LOG_ERROR("Unable to create texture from file: ", utils::narrow(textureNames[i]), ", error code: ", hr);
        }

        if (error)
        {
            LOG_ERROR("Unable to load texture in cubemap: ", utils::narrow(textureNames[i]));

            CComPtr<ID3D11Resource> resource = nullptr;
            errorTexture->texture->GetResource(&resource);
            resource->QueryInterface<ID3D11Texture2D>(&(srcTex[i]));
        }
    }

    D3D11_TEXTURE2D_DESC texElementDesc;
    srcTex[0]->GetDesc(&texElementDesc);

    D3D11_TEXTURE2D_DESC texArrayDesc;
    texArrayDesc.Width = texElementDesc.Width;
    texArrayDesc.Height = texElementDesc.Height;
    texArrayDesc.MipLevels = texElementDesc.MipLevels;
    texArrayDesc.ArraySize = elemsCount;
    texArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    texArrayDesc.SampleDesc.Count = 1;
    texArrayDesc.SampleDesc.Quality = 0;
    texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
    texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texArrayDesc.CPUAccessFlags = 0;
    texArrayDesc.MiscFlags = 0;

    CComPtr<ID3D11Texture2D> texArray = nullptr;
    if (FAILED(pDevice->CreateTexture2D(&texArrayDesc, 0, &texArray)))
    {
        LOG_ERROR("Unable to create cubemap texture");
        return nullptr;
    }

    // for each texture element...
    for (size_t i = 0; i < elemsCount; ++i)
    {
        // for each mipmap level...
        for (UINT j = 0; j < texElementDesc.MipLevels; ++j)
        {
            D3D11_MAPPED_SUBRESOURCE mappedTex2D;
            pDeviceContext->Map(srcTex[i], 0, D3D11_MAP_WRITE, 0, &mappedTex2D);
            pDeviceContext->UpdateSubresource(texArray, D3D11CalcSubresource(j, i, texElementDesc.MipLevels),
                                              0, mappedTex2D.pData, mappedTex2D.RowPitch, 0);
            pDeviceContext->Unmap(srcTex[i], 0);
        }
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = texArrayDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    viewDesc.Texture2DArray.MostDetailedMip = 0;
    viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
    viewDesc.Texture2DArray.FirstArraySlice = 0;
    viewDesc.Texture2DArray.ArraySize = elemsCount;

    CComPtr<ID3D11ShaderResourceView> view = nullptr;
    if (FAILED(pDevice->CreateShaderResourceView(texArray, &viewDesc, &view)))
        LOG_FATAL_ERROR("Unable to create ShaderResourceView");

    auto texturePtr = std::make_shared<D3D11Texture>(pDeviceContext, texElementDesc.Width, texElementDesc.Height, TextureType::TextureCubemap, view);
    textures[superString] = texturePtr;
    return texturePtr;
}

ITexturePtr D3D11TextureManager::CreateTextureArray(const std::vector<std::wstring> & names)
{
    if (names.size() < 2)
    {
        LOG_ERROR("Unable to load texture array with texture count < 2");
        return nullptr;
    }

    const size_t elemsCount = names.size();

    std::vector<DirectX::TexMetadata> metadatas;
    //check here info
    for (size_t i = 0; i < elemsCount; ++i)
    {
        DirectX::TexMetadata ii;
        if (FAILED(DirectX::GetMetadataFromWICFile((texturesPath + names[i]).c_str(), 0, ii)))
            LOG_ERROR("Unable to get image info for: ", utils::narrow(names[i]));

        metadatas.push_back(ii);
    }

    if (metadatas.size() > 1)
    {
        size_t prevWidth = metadatas[0].width;
        size_t prevHeight = metadatas[0].height;
        for (size_t i = 1; i < metadatas.size(); ++i)
            if (metadatas[i].width != prevWidth || metadatas[i].height != prevHeight)
                LOG_FATAL_ERROR("Unable to create texture array from textures without the same size");

    }

    std::vector<CComPtr<ID3D11Texture2D>> srcTex(elemsCount);

    for (size_t i = 0; i < elemsCount; ++i)
    {
        bool error = false;
        DirectX::ScratchImage image;
        if (FAILED(DirectX::LoadFromWICFile((texturesPath + names[i]).c_str(), 0, &metadatas[i], image)))
            error = true;

        if (FAILED(DirectX::CreateTextureEx(pDevice, 
                                            image.GetImage(0, 0, 0),
                                            1, 
                                            metadatas[i], 
                                            D3D11_USAGE_DEFAULT,
                                            D3D11_BIND_SHADER_RESOURCE,
                                            0,
                                            0,
                                            true, 
                                            (ID3D11Resource**)&srcTex[i])))
            error = true;

        if (error)
        {
            LOG_ERROR("Unable to load texture in texture array: ", utils::narrow(names[i]));

            // leakzzz
            CComPtr<ID3D11Resource> resource = nullptr;
            errorTexture->texture->GetResource(&resource);
            resource->QueryInterface<ID3D11Texture2D>(&(srcTex[i]));
        }
    }


    D3D11_TEXTURE2D_DESC texElementDesc;
    srcTex[0]->GetDesc(&texElementDesc);

    D3D11_TEXTURE2D_DESC texArrayDesc;
    texArrayDesc.Width = texElementDesc.Width;
    texArrayDesc.Height = texElementDesc.Height;
    texArrayDesc.MipLevels = texElementDesc.MipLevels;
    texArrayDesc.ArraySize = elemsCount;
    texArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    texArrayDesc.SampleDesc.Count = 1;
    texArrayDesc.SampleDesc.Quality = 0;
    texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
    texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texArrayDesc.CPUAccessFlags = 0;
    texArrayDesc.MiscFlags = 0;

    CComPtr<ID3D11Texture2D> texArray = nullptr;
    if (FAILED(pDevice->CreateTexture2D(&texArrayDesc, 0, &texArray)))
        LOG_ERROR("Unable to create TextureArray!");

    // for each texture element...
    for (size_t i = 0; i < elemsCount; ++i)
    {
        // for each mipmap level...
        for (UINT j = 0; j < texElementDesc.MipLevels; ++j)
        {
            D3D11_MAPPED_SUBRESOURCE mappedTex2D;
            pDeviceContext->Map(srcTex[i], 0, D3D11_MAP_WRITE, 0, &mappedTex2D);
            pDeviceContext->UpdateSubresource(texArray, D3D11CalcSubresource(j, i, texElementDesc.MipLevels),
                                              0, mappedTex2D.pData, mappedTex2D.RowPitch, 0);
            pDeviceContext->Unmap(srcTex[i], 0);
        }
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = texArrayDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    viewDesc.Texture2DArray.MostDetailedMip = 0;
    viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
    viewDesc.Texture2DArray.FirstArraySlice = 0;
    viewDesc.Texture2DArray.ArraySize = elemsCount;

    CComPtr<ID3D11ShaderResourceView> view = nullptr;
    if (FAILED(pDevice->CreateShaderResourceView(texArray, &viewDesc, &view)))
        LOG_FATAL_ERROR("Unable to create ShaderResourceView");

    auto texturePtr = std::make_shared<D3D11Texture>(pDeviceContext, texElementDesc.Width, texElementDesc.Height, TextureType::Texture2DArray, view);

    std::wstring superString = L"";
    for (auto & elem : names)
        superString += elem;

    textures[superString] = texturePtr;
    return texturePtr;
}

TextureInfo D3D11TextureManager::GetTextureInfo(const std::wstring & filename)
{
    DirectX::TexMetadata info;

    if (SUCCEEDED(DirectX::GetMetadataFromWICFile(filename.c_str(), 0, info)))
    {
        TextureInfo out;
        out.width = info.width;
        out.height = info.height;
        return out;
    }
    else
    {
        LOG_ERROR("Unable to get info for texture: ", utils::narrow(filename));
        return TextureInfo();
    }
}

void D3D11TextureManager::CreateErrorTexture()
{
    //trying to create texture procedurally
    const unsigned int width = 256;
    const unsigned int height = 256;
    CComPtr<ID3D11Texture2D> texture = nullptr;
    D3D11_TEXTURE2D_DESC description;
    description.ArraySize = 1;
    description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    description.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    description.Height = height;
    description.Width = width;
    description.MipLevels = 1;
    description.MiscFlags = 0;
    description.SampleDesc.Count = 1;
    description.SampleDesc.Quality = 0;
    description.Usage = D3D11_USAGE_DYNAMIC;

    if (FAILED(pDevice->CreateTexture2D(&description, nullptr, &texture)))
        LOG_FATAL_ERROR("Unable to create error texture");

    D3D11_MAPPED_SUBRESOURCE mappedTexture;
    if (FAILED(pDeviceContext->Map(texture, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, NULL, &mappedTexture)))
        LOG_FATAL_ERROR("Unable to create error texture");

    uint8_t * pTexels = (uint8_t*)mappedTexture.pData;
    for (UINT row = 0; row < height; row++)
    {
        UINT rowStart = row * mappedTexture.RowPitch;
        for (UINT col = 0; col < width; col++)
        {
            unsigned int xBlock = col * 8 / width;
            unsigned int yBlock = row * 8 / height;
            UINT colStart = col * 4;

            if (xBlock % 2 == yBlock % 2)
            {
                pTexels[rowStart + colStart + 0] = 255; // Red
                pTexels[rowStart + colStart + 1] = 0; // Green
                pTexels[rowStart + colStart + 2] = 0; // Blue
                pTexels[rowStart + colStart + 3] = 255; // Alpha
            }
            else
            {
                pTexels[rowStart + colStart + 0] = 0; // Red
                pTexels[rowStart + colStart + 1] = 255; // Green
                pTexels[rowStart + colStart + 2] = 0; // Blue
                pTexels[rowStart + colStart + 3] = 255; // Alpha
            }
        }
    }
    pDeviceContext->Unmap(texture, D3D11CalcSubresource(0, 0, 1));
    // Create the shader-resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
    srDesc.Format = description.Format;
    srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srDesc.Texture2D.MostDetailedMip = 0;
    srDesc.Texture2D.MipLevels = 1;

    CComPtr<ID3D11ShaderResourceView> pShaderResView = nullptr;

    if (FAILED(pDevice->CreateShaderResourceView(texture, &srDesc, &pShaderResView)))
        LOG_FATAL_ERROR("Unable to create error texture");

    errorTexture = std::make_shared<D3D11Texture>(pDeviceContext, width, height, TextureType::Texture2D, pShaderResView);
}

ITexturePtr D3D11TextureManager::GetErrorTexture()
{
    return errorTexture;
}
