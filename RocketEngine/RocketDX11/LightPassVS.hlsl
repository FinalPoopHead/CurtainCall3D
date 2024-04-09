static const float2 arrBasePos[4] =
{
    float2(-1.0, 1.0),
    float2(1.0, 1.0),
    float2(-1.0, -1.0),
    float2(1.0, -1.0),
};

static const float2 arrUV[4] =
{
    float2(0.0, 0.0),
    float2(1.0, 0.0),
    float2(0.0, 1.0),
    float2(1.0, 1.0)
};

struct PixelInput
{
	float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

PixelInput main(uint vertexID : SV_VertexID)
{
    PixelInput output = (PixelInput)0;
	
    output.position = float4(arrBasePos[vertexID].xy, 0.0, 1.0);
    output.uv = arrUV[vertexID].xy;
    
    return output;
}