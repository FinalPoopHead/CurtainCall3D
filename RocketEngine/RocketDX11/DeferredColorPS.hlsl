struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct DeferredOutput
{
    float4 position : SV_TARGET0;
    float4 baseColor : SV_TARGET1;
    float4 normal : SV_TARGET2;
};

DeferredOutput main(PixelInputType input)
{
    DeferredOutput output = (DeferredOutput)0;
    
    output.baseColor = input.color;
 
    return output;
}