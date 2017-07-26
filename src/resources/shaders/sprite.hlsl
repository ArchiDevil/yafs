cbuffer spriteConstants : register(b0)
{
    float4x4 WVPMatrix;
    float3x3 TextureMatrix;
    float4 MaskColor;
};

Texture2D Texture : register(t0);

SamplerState SS : register(s0);

struct VS_I
{
    float2 Position : POSITION;
    float2 Texcoord : TEXCOORD;
};

struct VS_O
{
    float4 Position : SV_POSITION;
    float2 Texcoord : TEXCOORD;
};

VS_O VS(VS_I Input)
{
    VS_O Output;
    Output.Position = mul(float4(Input.Position, 0.0f, 1.0f), WVPMatrix);
    float3 texCoord = mul(float3(Input.Texcoord, 1.0f), TextureMatrix);
    Output.Texcoord = texCoord.xy;
    return Output;
};

float4 PS(VS_O Input) : SV_TARGET
{
    return MaskColor * Texture.Sample(SS, Input.Texcoord);
};
