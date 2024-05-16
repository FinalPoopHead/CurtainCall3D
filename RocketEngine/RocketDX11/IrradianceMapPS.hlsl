TextureCube CubeMap : register(t0);
sampler LinearWrapSampler : register(s0);

static const float PI = 3.14159265359f;

float4 main(float3 wolrdPos : POSITION) : SV_TARGET
{
    float3 irradiance = float3(0.0f, 0.0f, 0.0f);
    float3 normal = normalize(wolrdPos);

    float3 up = float3(0.0f, 1.0f, 0.0f);
    float3 right = cross(up, normal);
    right = normalize(right);
    up = cross(normal, right);
    up = normalize(up);

    float sampleDelta = 0.025f;
    float nrSamples = 0.0f;
    for (float phi = 0.0f; phi < 2.0 * PI; phi += sampleDelta)
    {
        for (float theta = 0.0f; theta < 0.5 * PI; theta += sampleDelta)
        {
			// spherical to cartesian (in tangent space)
            float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			// tangent space to world
            float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
            
            irradiance += CubeMap.Sample(LinearWrapSampler, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples ++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    
    //irradiance.x *= 1.05f;
    //irradiance.y *= 0.965f;
    //irradiance.z *= 1.045f;
    
    irradiance = CubeMap.Sample(LinearWrapSampler, wolrdPos).rgb;
    irradiance = pow(irradiance, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
    
    return float4(irradiance, 1.0f);
}