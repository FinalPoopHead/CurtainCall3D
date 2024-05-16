TextureCube CubeMapTexture : register(t0);
SamplerState samplerLinear : register(s0);

struct DeferredOutput
{
    float4 position : SV_TARGET0;
    float4 baseColor : SV_TARGET1;
    float4 normal : SV_TARGET2;
};

DeferredOutput main(float3 worldPos : POSITION)
{
    DeferredOutput output = (DeferredOutput) 0;
    output.baseColor = CubeMapTexture.Sample(samplerLinear, worldPos);
    
    return output;
}