cbuffer ConstantBuffer : register(b0)
{
    float4x4 gWorldViewProj;
};

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT Input)
{
    VS_OUTPUT Output;

	// 출력을 위한 매트릭스를 만들어서 입력값에 곱해준다.
    Output.Position = mul(gWorldViewProj, float4(Input.Position.xyz, 1.0f));
    Output.UV = Input.UV;

    return Output;
}