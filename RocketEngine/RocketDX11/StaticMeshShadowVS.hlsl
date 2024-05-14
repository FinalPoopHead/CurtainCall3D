cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix worldInverse;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
    uint nodeIndex : BLENDINDICES;
};

float4 main(VertexInputType input) : SV_POSITION
{
    float4 output = (float4) 0;
    
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output = mul(float4(input.position, 1.0f), worldMatrix);
    output = mul(output, viewMatrix);
    output = mul(output, projectionMatrix);
    
    return output;
}