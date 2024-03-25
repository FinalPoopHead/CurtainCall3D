Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float4 specular;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    color = ambientColor;
    
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // 계산을 위해 빛 방향을 반전
    lightDir = -normalize(lightDirection);
    
    // 이 픽셀의 빛의 양을 계산
    lightIntensity = saturate(dot(input.normal, lightDir));
    
    if (lightIntensity > 0.0f)
    {
        // 빛의 강도와 결합 된 확산 색을 기준으로 최종 색상을 결정
        color += (diffuseColor * lightIntensity);
        
        color = saturate(color);
    
        // 반사된 빛의 방향을 계산
        float3 reflection = reflect(-lightDir, input.normal);
        
        // specular 계산 및 강도 조절
        specular = pow(max(dot(reflection, input.viewDirection), 0.0f), specularPower) * specularColor;
    }
    
    // 텍스처 픽셀과 최종 확산 색을 곱하여 최종 픽셀 색상 결과를 얻음
    color = color * textureColor;
    
    // 투명도 설정
    color.a = textureColor.a;

    // specular 색상 추가
    color = saturate(color + specular);
    
    return color;
}
