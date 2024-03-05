cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;
};

cbuffer NodeBuffer : register(b1)
{
    matrix nodeTransform[256];
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    uint nodeIndex : BLENDINDICES;
};
    
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDiretion : TEXCOORD1;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output = (PixelInputType) 0;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    
    matrix nodeTransformMatrix = nodeTransform[input.nodeIndex];
    
    //output.position = mul(input.position, mul(nodeTransformMatrix, worldMatrix));
    //output.position = mul(input.position, worldMatrix);
    output.position = mul(input.position, nodeTransformMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    
    output.normal = normalize(output.normal);
    
    float4 worldPosition = mul(input.position, worldMatrix);
    
    output.viewDiretion = cameraPosition.xyz - worldPosition.xyz;
    
    output.viewDiretion = normalize(output.viewDiretion);
    
    return output;
}