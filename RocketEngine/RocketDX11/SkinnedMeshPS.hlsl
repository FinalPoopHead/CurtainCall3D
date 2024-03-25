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
    
    // ����� ���� �� ������ ����
    lightDir = -normalize(lightDirection);
    
    // �� �ȼ��� ���� ���� ���
    lightIntensity = saturate(dot(input.normal, lightDir));
    
    if (lightIntensity > 0.0f)
    {
        // ���� ������ ���� �� Ȯ�� ���� �������� ���� ������ ����
        color += (diffuseColor * lightIntensity);
        
        color = saturate(color);
    
        // �ݻ�� ���� ������ ���
        float3 reflection = reflect(-lightDir, input.normal);
        
        // specular ��� �� ���� ����
        specular = pow(max(dot(reflection, input.viewDirection), 0.0f), specularPower) * specularColor;
    }
    
    // �ؽ�ó �ȼ��� ���� Ȯ�� ���� ���Ͽ� ���� �ȼ� ���� ����� ����
    color = color * textureColor;
    
    // ���� ����
    color.a = textureColor.a;

    // specular ���� �߰�
    color = saturate(color + specular);
    
    return color;
}
