Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

struct DeferredOutput
{
    float4 position : SV_TARGET0;
    float4 baseColor : SV_TARGET1;
    float4 normal : SV_TARGET2;
};

DeferredOutput main(PixelInputType input)
{
    DeferredOutput output;
    
    output.position = input.worldPosition;
    output.baseColor = shaderTexture.Sample(SampleType, input.tex);
    output.normal = float4(input.normal, 1.0f);
    
    return output;
}