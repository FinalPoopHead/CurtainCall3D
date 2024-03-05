struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float Opacity : COLOR;
};

struct PS_OUTPUT
{
    float4 depth : SV_Target0;
    float4 normal : SV_Target1;
    float4 albedo : SV_Target2;
    float4 specular : SV_Target3;
    float4 emissive : SV_Target4;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    ////return float4(0.8, 0.8, 0.8, input.Opacity);
    //PS_OUTPUT output;
    float4 color = float4(0.8f, 0.8f, 0.8f, input.Opacity);
    //float4 color = float4(0.8f, 0.8f, 0.8f, 1.0f);
    //float4 color = float4(input.Opacity, input.Opacity, input.Opacity, input.Opacity);
    //output.depth = color;
    //output.normal = color;
    //output.albedo = color;
    //output.specular = color;
    //output.emissive = color;
    
    return color;
}