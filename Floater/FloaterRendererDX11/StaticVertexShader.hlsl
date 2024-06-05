cbuffer ConstantBuffer : register(b0)
{
    float4x4 WorldViewProj;
    float4x4 WorldInvTransp;
};

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 UV0 : TEXCOORD0;
    //float2 UV1 : TEXCOORD1;
    //float2 UV2 : TEXCOORD2;
    //float2 UV3 : TEXCOORD3;
    //float2 UV4 : TEXCOORD4;
    //float2 UV5 : TEXCOORD5;
    //float2 UV6 : TEXCOORD6;
    //float2 UV7 : TEXCOORD7;
    //float2 UV8 : TEXCOORD8;
    //float2 UV9 : TEXCOORD9;
    float3 Normal : NORMAL;
    float3 binormal : BINORMAL;
    float3 tangent : TANGENT;
    uint BoneIndices[4] : BLENDINDICES;
    float BoneWeights[4] : BLENDWEIGHT;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    //float3 PositionW : POSITION;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
};

VS_OUTPUT main(VS_INPUT Input)
{
    VS_OUTPUT Output;
    
    Output.Position = mul(WorldViewProj, float4(Input.Position.xyz, 1.0f));
    
    Output.UV = Input.UV0;
    
    Output.Normal = normalize(mul((float3x3) WorldInvTransp, Input.Normal));

    return Output;
}