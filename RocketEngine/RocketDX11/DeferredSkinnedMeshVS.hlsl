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

cbuffer BoneBuffer : register(b2)
{
    matrix boneTransform[512];
}

struct VertexInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
    uint nodeIndex : BLENDINDICES0;
    float4 weights : BLENDWEIGHT;
    uint4 boneIndex : BLENDINDICES1;
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

PixelInputType main(VertexInputType input)
{
    PixelInputType output = (PixelInputType) 0;
    
    // Calculate the position of the vertex against the world, view, and projection matrices.
   
    matrix finalOffsetMatrix = mul(mul(boneTransform[input.boneIndex.x], nodeTransform[input.boneIndex.x]), input.weights.x) +
                               mul(mul(boneTransform[input.boneIndex.y], nodeTransform[input.boneIndex.y]), input.weights.y) +
                               mul(mul(boneTransform[input.boneIndex.z], nodeTransform[input.boneIndex.z]), input.weights.z) +
                               mul(mul(boneTransform[input.boneIndex.w], nodeTransform[input.boneIndex.w]), input.weights.w);
    
    float4 resultPosition = mul(float4(input.position, 1.0f), finalOffsetMatrix);
    resultPosition /= resultPosition.w;
    
    
    matrix nodeTransformMatrix = nodeTransform[input.nodeIndex];
    
    //output.position = mul(resultPosition, worldMatrix);
    output.position = mul(resultPosition, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    // TODO : offsetMatrix와 node matrix도 곱한 matrix의 역전치를 곱해야될거같은데 일단 보류
    output.normal = mul(float4(input.normal, 0.0f), finalOffsetMatrix).xyz;
    output.normal = normalize(output.normal);
    output.tangent = mul(float4(input.tangent, 0.0f), finalOffsetMatrix).xyz;
    output.tangent = normalize(output.tangent);
    output.bitangent = mul(float4(input.bitangent, 0.0f), finalOffsetMatrix).xyz;
    output.bitangent = normalize(output.bitangent);
    
    output.worldPosition = resultPosition;
    
    return output;
}