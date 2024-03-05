cbuffer ConstantBuffer : register(b0)
{
    float4x4 WorldViewProj;
};

//cbuffer ConstantBuffer : register(b0)
//{
//    float4x4 World;
//    float4x4 WorldInvTransp;
//    float4x4 ViewProj;
//    float4x4 WorldViewProj;
//};

//cbuffer BoneConstantBuffer : register(b1)
//{
//    float4x4 BoneMatrices[96];
//};

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 UV0 : TEXCOORD;
    float2 UV1 : TEXCOORD;
    float2 UV2 : TEXCOORD;
    float2 UV3 : TEXCOORD;
    float2 UV4 : TEXCOORD;
    float2 UV5 : TEXCOORD;
    float2 UV6 : TEXCOORD;
    float2 UV7 : TEXCOORD;
    float2 UV8 : TEXCOORD;
    float2 UV9 : TEXCOORD;
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
    //float2 UV : TEXCOORD;
    //float3 Normal : NORMAL;
};

VS_OUTPUT main(VS_INPUT Input)
{
    VS_OUTPUT Output;

    //float4x4 bone = Input.BoneWeights[0] * BoneMatrices[Input.BoneIndices[0]];
    //bone += Input.BoneWeights[1] * BoneMatrices[Input.BoneIndices[1]];
    //bone += Input.BoneWeights[2] * BoneMatrices[Input.BoneIndices[2]];
    //bone += Input.BoneWeights[3] * BoneMatrices[Input.BoneIndices[3]];
    
    //if (Input.BoneWeights[0] + Input.BoneWeights[1] + Input.BoneWeights[2] + Input.BoneWeights[3] == 0)
    //{
    //    bone =
    //    float4x4
    //    (
    //    1, 0, 0, 0,
    //    0, 1, 0, 0,
    //    0, 0, 1, 0,
    //    0, 0, 0, 1
    //    );
    //}
    
    //float4x4 bone =
    //    float4x4
    //    (
    //    1.0f, 0, 0, 0,
    //    0, 1.0f, 0, 0,
    //    0, 0, 1.0f, 0,
    //    0, 0, 0, 1.0f
    //    );
    
    //float4 PosL = mul(bone, float4(Input.Position.xyz, 1.0f));
    Output.Position = mul(WorldViewProj, float4(Input.Position.xyz, 1.0f));
    //Output.PositionW = mul(World, PosL);
    //// 아래 임시코드
    //Output.PositionW = PosL;
    
    //Output.UV = Input.UV0;
    
    //float3 normalL = mul((float3x3) bone, Input.Normal);
    ////Output.Normal = mul((float3x3) WorldInvTransp, normalL);
    ////Output.Normal = normalize(Output.Normal);
    //// 아래 임시코드
    //Output.Normal = normalize(normalL);

    return Output;
}