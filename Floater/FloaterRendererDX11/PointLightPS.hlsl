cbuffer ConstantBuffer : register(b0)
{
    float4x4 view;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D Depth : register(t0);
Texture2D Normal : register(t1);
Texture2D Albedo : register(t2);

SamplerState Sampler : register(s0);

float4 main(VS_OUTPUT input) : SV_Target
{
    float3 normal = Normal.Sample(Sampler, input.uv).xyz;
    float4 albedo = Albedo.Sample(Sampler, input.uv);
    float4 depth = Depth.Sample(Sampler, input.uv);
    
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    if (length(normal) > 0.0f)
    {
        float3 lightDir = normalize(float3(1.0f, 1.0f, 1.0f));
        float3 viewPos = float3(view._41, view._42, view._43);
        
        float lambertian = max(dot(normal, lightDir), 0.0f);
        float specular = 0.0f;
        
        [flatten]
        if (lambertian > 0.0f)
        {
            float3 viewDir = normalize(-viewPos);
            float3 halfDir = normalize(lightDir + viewDir);
            float specAngle = max(dot(halfDir, normal), 0.0f);
            specular = pow(specAngle, 32.0f);
        }
        
        color.xyz = lambertian * albedo.rgb + specular * float3(1.0f, 1.0f, 1.0f);
    }
    
    return color;
}