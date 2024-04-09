cbuffer MatrixBuffer : register(b0)
{
    matrix view;
    matrix projection;
};

struct PixelInputType
{
    float3 worldPos : POSITION;
    float4 pos : SV_POSITION;
};

PixelInputType main(float3 pos : POSITION)
{
    PixelInputType output = (PixelInputType) 0;
    
    output.worldPos = pos;
    
    output.pos = mul(float4(pos, 0.0f), view);
    output.pos = mul(output.pos, projection);
    
    output.pos.z = output.pos.w;
    
    return output;
}