#ifndef PBR_FUNCTION
#define PBR_FUNCTION
#endif

#ifndef PI
#define PI 3.14159265359
#endif



float3 Disney_Diffuse(in float roughnessPercent, in float3 diffuseColor, in float NdotL, in float NdotV, in float LdotH)
{
    float energyBias = lerp(0.0f, 0.5f, roughnessPercent);
    float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughnessPercent);
   
    float fd90 = energyBias + 2.0f * roughnessPercent * LdotH * LdotH;
    
    float lightScatter = 1.0f + (fd90 - 1.0f) * pow(1.0f - NdotL, 5.0f);
    float viewScatter = 1.0f + (fd90 - 1.0f) * pow(1.0f - NdotV, 5.0f);
    
    return diffuseColor * lightScatter * viewScatter * energyFactor;
}

// GGX Specular D
float Specular_D_GGX(in float roughness, in float NdotH)
{
    float a = roughness * roughness;
    const float lower = (NdotH * (NdotH * a - NdotH)) + 1.0f;
    return a / (PI * lower * lower);
}

float DistributionGGX(float roughness, float NdotH)
{
    float a2 = roughness * roughness;
    float NdotH2 = NdotH * NdotH;
	
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0f);
    denom = PI * denom * denom;
	
    return nom / denom;
}

// Schlick-Smith specular G (visibility) By Unity Version
float Specular_G_Smith_Unity(float roughness, float NdotV, float NdotL)
{
    float a = roughness * roughness;
    float SmithV = NdotL * sqrt(NdotV * (NdotV - NdotV * a) + a);
    float SmithL = NdotV * sqrt(NdotL * (NdotL - NdotL * a) + a);
    
    return 0.5f / max(SmithV + SmithL, 1e-5f);
}

float GeometrySchlickGGX(float NdotV, float roughness, int isIBL = 0)  // k is a remapping of roughness based on direct lighting or IBL lighting
{
    // For Directionl Light
    float r = roughness + 1.0f;
    float notIBL_k = (r * r) / 8.0f;

    // For IBL
    float IBL_k = (roughness * roughness) / 2.0f;
    
    float k = notIBL_k * (1 - isIBL) + IBL_k * isIBL;

    float nom = NdotV;
    float denom = max(NdotV * (1.0f - k) + k, 1e-5f);

    return nom / denom;
}
  
float GeometrySmith(float NdotV, float NdotL, float roughness, int isIBL = 0)
{
    float ggx2 = GeometrySchlickGGX(NdotV, roughness, isIBL);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness, isIBL);

    return ggx1 * ggx2;
}

// Shlick's approximation of Fresnel By Unity Engine
float3 Specular_F_Fresnel_Shlick_Unity(in float3 specularColor, in float NdotV)
{
    float FC = pow(saturate(1.0f - NdotV), 5.0f);
    return specularColor + (1.0f - specularColor) * FC;
}

float3 FresnelSchlickRoughness(in float3 specularColor, in float NdotV, in float roughness)
{
    float a = 1.0f - roughness;
    float FC = pow(saturate(1.0f - NdotV), 5.0f);
    return specularColor + (max(float3(a, a, a), specularColor) - specularColor) * FC;
}

float3 Specular_BRDF(in float roughness, in float3 specularColor, in float NdotH, in float NdotV, in float NdotL, in float LdotH)
{
    // Specular D
    float specular_D = Specular_D_GGX(roughness, NdotH);
    
    // Specular G
    float specular_G = Specular_G_Smith_Unity(roughness, NdotV, NdotL);
    
    // Specular F
    float3 specular_F = Specular_F_Fresnel_Shlick_Unity(specularColor, NdotV);

    return (specular_D * specular_G) * specular_F;
}