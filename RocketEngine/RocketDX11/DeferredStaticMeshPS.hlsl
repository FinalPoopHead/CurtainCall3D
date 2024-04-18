Texture2D baseColorTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D metallicTexture : register(t2);
Texture2D roughnessTexture : register(t3);
Texture2D ambientOcclusionTexture : register(t4);
SamplerState SampleType : register(s0);

cbuffer PBRBuffer : register(b0)
{
    float metallic;
    float roughness;
    
    int useNormalMap;
    int useMetallicMap;
    int useRoughnessMap;
    int useAOMap;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
    float3 viewDiretion : TEXCOORD1;
};

struct DeferredOutput
{
    float4 position : SV_TARGET0;
    float4 baseColor : SV_TARGET1;
    float4 normal : SV_TARGET2;
    float4 metallic : SV_Target3;
    float4 roughness : SV_Target4;
    float4 ambientOcclusion : SV_Target5;
};

DeferredOutput main(PixelInputType input)
{
    DeferredOutput output;
    
    float3 normalMap = normalTexture.Sample(SampleType, input.tex).xyz;
    float4 tangentNormal;
    
    normalMap = (normalMap * 2.0f) - 1.0f;
    tangentNormal = float4((normalMap.r * input.tangent) + (normalMap.g * input.bitangent) + (normalMap.b * input.normal), 1.0f);
    tangentNormal = normalize(tangentNormal);
    
    output.position = input.worldPosition;
    output.baseColor = baseColorTexture.Sample(SampleType, input.tex);
    output.normal = (float4(input.normal, 1.0f) * (1 - useNormalMap))
                    + (tangentNormal * useNormalMap);
    
    output.metallic = (float4(metallic, metallic, metallic, 1.0f) * (1 - useMetallicMap))
                    + (metallicTexture.Sample(SampleType, input.tex) * useMetallicMap);
    
    output.roughness = float4(roughness, roughness, roughness, 1.0f) * (1 - useRoughnessMap)
                    + (roughnessTexture.Sample(SampleType, input.tex) * useRoughnessMap);
    
    output.ambientOcclusion = float4(1.0f, 1.0f, 1.0f, 1.0f) * (1 - useAOMap)
                            + (ambientOcclusionTexture.Sample(SampleType, input.tex) * useAOMap);
    
    output.normal.w = 1.0f;
    output.metallic.w = 1.0f;
    output.roughness.w = 1.0f;
    output.ambientOcclusion.w = 1.0f;
    
    return output;
}