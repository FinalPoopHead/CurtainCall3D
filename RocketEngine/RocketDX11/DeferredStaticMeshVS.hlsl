cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix worldInverse;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer : register(b1)
{
    float3 cameraPosition;
    float padding;
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
    
struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
    float3 viewDiretion : TEXCOORD1;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output = (PixelInputType) 0;
    
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(float4(input.position, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    output.normal = mul(float4(input.normal, 0.0f), (float4x4) transpose(worldInverse));
    output.normal = normalize(output.normal);
    
    // float4 worldPosition = mul(float4(input.position, 1.0f), mul(nodeTransformMatrix, worldMatrix));
    float4 worldPosition = mul(float4(input.position, 1.0f), worldMatrix);
    
    output.viewDiretion = cameraPosition.xyz - worldPosition.xyz;
    
    output.viewDiretion = normalize(output.viewDiretion);
    
    output.worldPosition = worldPosition;
    
    return output;
}