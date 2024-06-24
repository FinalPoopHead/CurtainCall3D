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

// Model �������� StaticMesh�� �׸��� ����� Vertex Shader
// NodeTransform �� �Ѱ��༭ ����Ѵ�.
float4 main(VertexInputType input) : SV_POSITION
{
    float4 output = (float4) 0;
    
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output = mul(float4(input.position, 1.0f), worldMatrix);
    output = mul(output, viewMatrix);
    output = mul(output, projectionMatrix);
    
    return output;
}

/*
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix worldInverse;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer NodeBuffer : register(b1)
{
    matrix nodeTransform[512];
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

// Model �������� StaticMesh�� �׸��� ����� Vertex Shader
// NodeTransform �� �Ѱ��༭ ����Ѵ�.
float4 main(VertexInputType input) : SV_POSITION
{
    float4 output = (float4) 0;
    
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output = mul(float4(input.position, 1.0f), nodeTransform[input.nodeIndex]);
    output = mul(output, worldMatrix);
    output = mul(output, viewMatrix);
    output = mul(output, projectionMatrix);
    
    return output;
}
*/