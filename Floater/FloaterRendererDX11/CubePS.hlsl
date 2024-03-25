
struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
};

Texture2D textureMap : register(t0);
SamplerState g_Sampler : register(s0);

struct PS_OUTPUT
{
    float4 depth : SV_Target0;
    float4 normal : SV_Target1;
    float4 albedo : SV_Target2;
    float4 specular : SV_Target3;
    float4 emissive : SV_Target4;
};

PS_OUTPUT main(VS_OUTPUT input)
{
    ////return float4(0.8, 0.8, 0.8, input.Opacity);
    PS_OUTPUT output;
    float4 color = textureMap.Sample(g_Sampler, input.UV);
    output.depth = color;
    output.normal = color;
    output.albedo = color;
    output.specular = float4(0.0f, 0.0f, 0.0f, 1.0f);
    output.emissive = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    return output;
}