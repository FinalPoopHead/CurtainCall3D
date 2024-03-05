struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_OUTPUT main(uint id : SV_VertexID)
{
    VS_OUTPUT output;
    
    output.uv = float2((id << 1) & 2, id & 2);
    
    output.position = float4(output.uv * float2(2, -2) + float2(-1, 1), 0, 1);
    
    return output;
}