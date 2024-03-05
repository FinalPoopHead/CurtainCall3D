cbuffer ConstantBuffer : register(b0)
{
    float width;
    float height;
    float x;
    float y;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.uv = input.UV;
    output.position = float4((input.Position.x * width) + x - 0.5f, (input.Position.y * height) + (1 - y) - 0.5, input.Position.z, 1.0f);
    return output;
}