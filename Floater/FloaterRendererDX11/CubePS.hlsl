
struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
};

Texture2D textureMap : register(t0);
SamplerState g_Sampler : register(s0);

float4 main(VS_OUTPUT input) : SV_Target
{
	// �ؽ�ó ���ø�
	//float4 texColor = (0.5, 0.5, 0, 0.9);
    float4 texColor = textureMap.Sample(g_Sampler, input.UV);
	//float4 texColor = {input.UV, 0, 1};

    return texColor;
}