float main(float4 input : SV_POSITION) : SV_TARGET
{
//    float depthValue = input.z / input.w;
//    float4 output = float4(depthValue, depthValue, depthValue, 1.0f);
    return input.z / input.w;
}