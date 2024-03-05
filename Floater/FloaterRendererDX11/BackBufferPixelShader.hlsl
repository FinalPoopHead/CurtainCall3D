struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D Tex0 : register(t0);
Texture2D Tex1 : register(t1);
Texture2D Tex2 : register(t2);
Texture2D Tex3 : register(t3);
Texture2D Tex4 : register(t4);
SamplerState Sampler : register(s0);

float4 main(VS_OUTPUT input) : SV_Target
{
    float4 texColor = Tex0.Sample(Sampler, input.uv);
    return texColor;
}