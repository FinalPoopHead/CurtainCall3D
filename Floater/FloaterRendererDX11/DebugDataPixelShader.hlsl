struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

float4 main(VS_OUTPUT input) : SV_Target
{
    float4 texColor = Texture.Sample(Sampler, input.uv);
    return texColor;
}