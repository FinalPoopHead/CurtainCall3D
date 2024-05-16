TextureCube CubeMapTexture : register(t0);
SamplerState samplerLinear : register(s0);

float4 main(float3 worldPos : POSITION) : SV_TARGET
{
    float3 color = CubeMapTexture.Sample(samplerLinear, worldPos).rgb;
    //color = color / (color + float3(1.0f, 1.0f, 1.0f));
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
    return float4(color.rgb, 1.0f);
}