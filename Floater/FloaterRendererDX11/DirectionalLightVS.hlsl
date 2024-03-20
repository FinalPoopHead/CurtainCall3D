struct VS_INPUT
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.uv = input.UV;
    output.position = float4(input.Position, 1.0f);
    return output;
}