SamplerState BlitPassSampler : register(s0);
Texture2D src : register(t0);

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(PixelInput input) : SV_TARGET
{
    float2 texCoord = input.uv;
    return src.Sample(BlitPassSampler, texCoord);
}