#include "PBRFunction.hlsli"

#ifndef MAX_REF_LOD
#define MAX_REF_LOD 10.0f
#endif

SamplerState LightPassSampler : register(s0);
SamplerState CubeMapSampler : register(s1);
SamplerState ShadowMapSampler : register(s2);

Texture2D Position : register(t0);
Texture2D BaseColor : register(t1);
Texture2D Normal : register(t2);
Texture2D Metallic : register(t3);
Texture2D Roughness : register(t4);
Texture2D AmbientOcclusion : register(t5);
TextureCube IBLIrradiance : register(t6);
TextureCube IBLPrefilter : register(t7);
Texture2D IBLBRDFLUT : register(t8);
Texture2D ShadowMap : register(t9);

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

cbuffer shadowBuffer : register(b2)
{
    float4x4 lightViewProjection;
}

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float CalculateShadowFactor(float3 position)
{
    float bias = 0.001f;    // 부동 소수점 정밀도 문제를 해결할 바이어스 값?
    
    float4 lightSpacePos = mul(float4(position, 1.0f), lightViewProjection);
    float3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    float currentDepth = projCoords.z;
    
    if (currentDepth > 1.0f)
    {
        return 1.0f;
    }

    projCoords = projCoords * 0.5f + 0.5f;      // -1 ~ 1 -> 0 ~ 1
    
    float depthValue = ShadowMap.Sample(ShadowMapSampler, float2(projCoords.x, 1 - projCoords.y)).r;
    
    float shadowFactor = 0.0f;  // 1.0f이면 빛을 받고 있음. 0.0f이면 아예 안받음
    
    if(currentDepth - bias < depthValue)
    {
        shadowFactor = 1.0f;        // 임시로 그냥 때려박은 값
    }
    
    return shadowFactor;
}

float4 main(PixelInput input) : SV_TARGET
{
    float gamma = 2.2f;
    
    float3 posW = Position.Sample(LightPassSampler, input.uv).rgb;
    float3 baseColor = BaseColor.Sample(LightPassSampler, input.uv).rgb;
    // Gamma Space -> Linear Space
    baseColor = pow(baseColor, gamma);
    float3 normal = Normal.Sample(LightPassSampler, input.uv).rgb;
    normal = normalize(normal);
    float metallic = Metallic.Sample(LightPassSampler, input.uv).r;
    metallic = saturate(metallic);
    float roughness = Roughness.Sample(LightPassSampler, input.uv).r;
    roughness = saturate(roughness);
    float ambientOcclusion = AmbientOcclusion.Sample(LightPassSampler, input.uv).r;
    ambientOcclusion = saturate(ambientOcclusion);
    
    
    float3 lightDir = -normalize(lightDirection);
    float3 viewDir = normalize(viewPosition - posW);
    viewDir = normalize(viewDir);
    float3 halfVector = normalize(lightDir + viewDir);
    
    float NdotL = max(dot(normal, lightDir), 0.0f);
    float NdotV = max(dot(normal, viewDir), 0.0f);
    float LdotH = max(dot(lightDir, halfVector), 0.0f);
    float NdotH = max(dot(normal, halfVector), 0.0f);
    float VdotH = max(dot(viewDir, halfVector), 0.0f);
   
    float3 lightColor = float3(1.0f, 1.0f, 1.0f);
    float3 radiance = lightColor * NdotL;
    float lightPower = 3.5f;
    
    // PBR
    float3 F0 = { 0.04f, 0.04f, 0.04f };
    F0 = lerp(F0, baseColor, metallic);
    float3 specularColor = F0;
    
    //float D = Specular_D_GGX(roughness.x, NdotH); // ->0
    float D = DistributionGGX(roughness, NdotH);
    float G = GeometrySmith(NdotV, NdotL, roughness, 1);
    float3 F = Specular_F_Fresnel_Shlick_Unity(specularColor, VdotH);
    float denominator = max((4 * NdotV * NdotL), 0.0001f);
    
    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    kD *= (1.0 - metallic);
    
    float3 BRDFspecular = D * G * F / denominator;
    
    float4 outputColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float shadowFactor = CalculateShadowFactor(posW);
    //shadowFactor *= 0.0001f;
    //shadowFactor += 1.0f;
    outputColor.rgb = ((kD * baseColor / PI) + BRDFspecular) * radiance * NdotL * shadowFactor * lightPower;
 
    // IBL
    float3 irradiance = IBLIrradiance.Sample(CubeMapSampler, normal).rgb;
    float3 prefilteredColor = IBLPrefilter.SampleLevel(CubeMapSampler, reflect(-viewDir, normal), roughness * MAX_REF_LOD).rgb;
    // OpenGL LUT 사용해서 1-roughness 하는 중, 내가 직접 만들어서 roughness로 바꿀 것.
    float2 brdf = IBLBRDFLUT.Sample(LightPassSampler, float2(NdotV, 1 - roughness)).rg;
    
    kS = FresnelSchlickRoughness(specularColor, NdotV, roughness);
    kD = float3(1.0f, 1.0f, 1.0f) - kS;
    kD *= (1.0 - metallic);
    float3 diffuse = irradiance * baseColor;
    
    float3 IBLdiffuse = irradiance * baseColor * kD;
    float3 IBLspecular = prefilteredColor * (kS * brdf.x + brdf.y);
    
    outputColor.xyz += (IBLdiffuse + IBLspecular) * ambientOcclusion * 0.3f;
    
    // Tone Mapping
    // outputColor = outputColor / (outputColor + float4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // Gamma Correction
    outputColor = pow(outputColor, float4(1.0f / gamma, 1.0f / gamma, 1.0f / gamma, 1.0f / gamma));
    
    // 임시 GlobalAmbient
//    float3 globalAmbient = float3(0.1f, 0.1f, 0.1f); // 임시값
//    float3 ambient = globalAmbient * baseColor * ambientOcclusion;
    
    outputColor.w = 1.0f;
    return outputColor;
    //return float4(ambient + outputColor.xyz, 1.0f);
}