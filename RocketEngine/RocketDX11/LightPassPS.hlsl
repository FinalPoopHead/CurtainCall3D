SamplerState SampleType;

Texture2D Position : register(t0);
Texture2D BaseColor : register(t1);
Texture2D Normal : register(t2);

cbuffer LightBuffer
{
    float3 lightDirection;
    float padding;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(PixelInput input) : SV_TARGET
{ 
    float3 posW = Position.Sample(SampleType, input.uv).rgb;
    float3 baseColor = BaseColor.Sample(SampleType, input.uv).rgb;
    float3 normal = Normal.Sample(SampleType, input.uv).rgb;
    
    float3 lightDir = -lightDirection;
    float lightIntensity = saturate(dot(normal.xyz, lightDir));
    
    float4 outputColor = saturate(float4(baseColor, 1.0f) * lightIntensity);
        
    return outputColor;
}