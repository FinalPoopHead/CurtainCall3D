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

float4 main(VertexInputType input) : SV_POSITION
{
    float4 output = (float4) 0;
    
    // Calculate the position of the vertex against the world, view, and projection matrices.
   
    matrix finalOffsetMatrix = mul(mul(boneTransform[input.boneIndex.x], nodeTransform[input.boneIndex.x]), input.weights.x) +
                               mul(mul(boneTransform[input.boneIndex.y], nodeTransform[input.boneIndex.y]), input.weights.y) +
                               mul(mul(boneTransform[input.boneIndex.z], nodeTransform[input.boneIndex.z]), input.weights.z) +
                               mul(mul(boneTransform[input.boneIndex.w], nodeTransform[input.boneIndex.w]), input.weights.w);
    
    float4 resultPosition = mul(float4(input.position, 1.0f), finalOffsetMatrix);
    
    // weight 값의 총 합이 1이 아닌 경우 동차좌표인 1이 변조가 된다.
    // 따라서 w가 변조된 경우 다시 1로 세팅해 주는 것.
    
    output = mul(resultPosition, viewMatrix);
    output = mul(output, projectionMatrix);

    return output;
}