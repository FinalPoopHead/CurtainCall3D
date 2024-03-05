cbuffer EntityInitData : register(b0)
{
    uint gNumRows;
};

cbuffer FramePerCamera : register(b1)
{
    float3 gCameraPosition;
}

cbuffer FramePerEntity : register(b2)
{
    matrix gWorldTranslateMatrix;
    matrix gWorldViewProj;
    float gHightOpacity;
}


struct VS_INPUT
{
    float3 Position : POSITION;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float Opacity : COLOR;
};

VS_OUTPUT main(VS_INPUT Input)
{
    VS_OUTPUT output;

	// 출력을 위한 매트릭스를 만들어서 입력값에 곱해준다.
    float3 worldPosition = mul(gWorldTranslateMatrix, float4(Input.Position.xyz, 1.0f)).xyz;
    float2 toCamera = gCameraPosition.xz - worldPosition.xz;
    float distance = length(toCamera);
    float div = (distance) / (gNumRows / 3);
    float clamped = clamp(div, 0.0f, 1.0f);
    output.Opacity = 1.0f - clamped;
    output.Opacity *= gHightOpacity;
    
    output.Position = mul(gWorldViewProj, float4(Input.Position.xyz, 1.0f));
    return output;
}