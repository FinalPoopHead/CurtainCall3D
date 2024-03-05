Texture2D textureMap : register(t0);
SamplerState g_Sampler : register(s0);


struct VS_OUTPUT
{
    float4 Position : SV_Position;
    //float3 PositionW : POSITION;
    //float2 UV : TEXCOORD;
    //float3 Normal : NORMAL;
};

struct PS_OUTPUT
{
    float4 depth : SV_Target0;
    float4 normal : SV_Target1;
    float4 albedo : SV_Target2;
    float4 specular : SV_Target3;
    float4 emissive : SV_Target4;
};

PS_OUTPUT main(VS_OUTPUT input) : SV_Target
{
    PS_OUTPUT output;

    //float4 texColor = textureMap.Sample(g_Sampler, input.UV);
    //float4 finalColor = texColor;
    float4 color = normalize(input.Position);
    output.depth = float4(color.xyz, 1);
    output.normal = float4(color.xxx, 1);
    output.albedo = float4(color.yyy, 1);
    output.specular = float4(color.zzz, 1);
    output.emissive = float4(color.www, 1);
    return output;
}