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
    
struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
};

// Model 기준으로 StaticMesh를 그릴때 사용할 Vertex Shader
// NodeTransform 을 넘겨줘서 사용한다.
PixelInputType main(VertexInputType input)
{
    PixelInputType output = (PixelInputType) 0;
    
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(float4(input.position, 1.0f), worldMatrix);
    output.worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    float4x4 inverseTranspose = (float4x4) transpose(worldInverse);
    
    output.normal = mul(float4(input.normal, 0.0f), inverseTranspose).xyz;
    output.normal = normalize(output.normal);
    output.tangent = mul(float4(input.tangent, 0.0f), inverseTranspose).xyz;
    output.tangent = normalize(output.tangent);
    output.bitangent = mul(float4(input.bitangent, 0.0f), inverseTranspose).xyz;
    output.bitangent = normalize(output.bitangent);
    
    return output;
}