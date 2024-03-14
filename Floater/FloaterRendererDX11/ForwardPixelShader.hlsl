Texture2D textureMap : register(t0);
SamplerState g_Sampler : register(s0);

struct VS_OUTPUT
{
	float4 Position : SV_Position;
    float2 UV : TEXCOORD;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 texColor = textureMap.Sample(g_Sampler, input.UV);
    return texColor;
}