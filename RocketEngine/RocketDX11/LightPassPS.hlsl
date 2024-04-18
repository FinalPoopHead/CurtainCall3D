#include "PBRFunction.hlsli"

#ifndef MAX_REF_LOD
#define MAX_REF_LOD 4.0
#endif

SamplerState LightPassSampler : register(s0);
SamplerState CubeMapSampler : register(s1);

Texture2D Position : register(t0);
Texture2D BaseColor : register(t1);
Texture2D Normal : register(t2);
Texture2D Metallic : register(t3);
Texture2D Roughness : register(t4);
Texture2D AmbientOcclusion : register(t5);
TextureCube IBLIrradiance : register(t6);
TextureCube IBLPrefilter : register(t7);
Texture2D IBLBRDFLUT : register(t8);

cbuffer LightBuffer : register(b0)
{
    float3 lightDirection;
    float padding;
};

cbuffer viewBuffer : register(b1)
{
    float3 viewPosition;
    float padding1;
}

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(PixelInput input) : SV_TARGET
{
    float gamma = 2.2f;
    
    float3 posW = Position.Sample(LightPassSampler, input.uv).rgb;
    float3 baseColor = BaseColor.Sample(LightPassSampler, input.uv).rgb;
    // Gamma Space -> Linear Space
    baseColor = pow(baseColor, gamma);
    float3 normal = Normal.Sample(LightPassSampler, input.uv).rgb;
    normal = normalize(normal);
    float3 metallic = Metallic.Sample(LightPassSampler, input.uv).rgb;
    float3 roughness = Roughness.Sample(LightPassSampler, input.uv).rgb;
    roughness = max(roughness, 0.04f);
    float3 ambientOcclusion = AmbientOcclusion.Sample(LightPassSampler, input.uv).rgb;
    
    float3 lightDir = -lightDirection;
    float lightIntensity = saturate(dot(normal.xyz, lightDir));
    
    float3 viewDir = normalize(viewPosition - posW);
    float3 halfVector = normalize(lightDir + viewDir);
   
    float NdotL = max(dot(normal, lightDir), 0.0f);
    float NdotV = max(dot(normal, viewDir), 0.0f);
    float LdotH = max(dot(lightDir, halfVector), 0.0f);
    float NdotH = max(dot(normal, halfVector), 0.0f);
    
    // PBR
    float3 F0 = 0.04f;
    F0 = lerp(F0, baseColor, metallic);
    float3 specularColor = F0;
    float3 lightColor = float3(1.0f, 1.0f, 1.0f);
    
    float D = Specular_D_GGX(roughness.x, NdotH);
    float G = GeometrySmith(normal, viewDir, lightDir, roughness.x, 1);
    float3 F = Specular_F_Fresnel_Shlick_Unity(specularColor, LdotH);
    float denominator = max((4 * NdotV * NdotL), 0.00001f);
    
    float3 BRDFspecular = D * G * F / denominator;
    
    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    kD *= (1.0 - metallic.x);
    
    float4 outputColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    outputColor = float4(((kD * baseColor / PI) + BRDFspecular) * lightColor * NdotL, 1.0f);
    //outputColor *= 0.001f;
    //float4 outputColor = float4((Disney_Diffuse(roughness.x,baseColor,NdotL,NdotV,LdotH) + BRDFspecular) * lightColor * NdotL, 1.0f);
    
    // IBL
    float3 globalAmbient = float3(0.3f, 0.3f, 0.3f); // ÀÓ½Ã°ª
    float3 ambient = globalAmbient * baseColor * ambientOcclusion;
    
    float3 irradiance = IBLIrradiance.Sample(CubeMapSampler, normal).rgb;
    float3 prefilteredColor = IBLPrefilter.SampleLevel(CubeMapSampler, reflect(-viewDir, normal), roughness.x * MAX_REF_LOD).
    rgb;
    float2 brdf = IBLBRDFLUT.Sample(LightPassSampler, float2(NdotV, roughness.x)).rg;
    
    kS = FresnelSchlickRoughness(F0, NdotV, roughness.x);
    kD = float3(1.0f, 1.0f, 1.0f) - kS;
    kD *= (1.0 - metallic.x);
    
    float3 IBLdiffuse = irradiance * baseColor * kD;
    float3 IBLspecular = prefilteredColor * (kS * brdf.x + brdf.y);
    
    outputColor.xyz += (IBLdiffuse + IBLspecular) * ambientOcclusion;
    
    // Tone Mapping
    // outputColor = outputColor / (outputColor + float4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // Gamma Correction
    outputColor = pow(outputColor, float4(1.0f / gamma, 1.0f / gamma, 1.0f / gamma, 1.0f / gamma));
    
    outputColor.w = 1.0f;
    return outputColor;
    //return float4(ambient + outputColor.xyz, 1.0f);
}