TextureCube CubeMapTexture : register(t0);
SamplerState samplerLinear : register(s0);

float4 main(float3 worldPos : POSITION) : SV_TARGET
{
    return CubeMapTexture.Sample(samplerLinear, worldPos);
}