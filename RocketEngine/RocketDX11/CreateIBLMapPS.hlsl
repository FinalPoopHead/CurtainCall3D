/*
TextureCube CubeMapTexture : register(t0);
SamplerState samplerLinear : register(s0);

const float2 invAtan = float2(0.1591f, 0.3183f);
float2 SampleSphericalMap(float3 v)
{
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5f;
    return uv;
}

float4 PS(VertexOutput input) : SV_TARGET
{
    float2 uv = SampleSphericalMap(normalize(input.oPosition.xyz));
    float3 envColor = CubeMapTexture.Sample(samplerLinear, uv);

    //  HDR -> LDR(Tone map)
    envColor = envColor / (envColor + float3(1.0f, 1.0f, 1.0f));
    //  Gamma Correction
    envColor = pow(envColor, 1.0f / 2.2f);

    return float4(envColor, 1.0f);
}

const static float PI = 3.14159265359;
float4 main(VertexOutput input) : SV_Target
{
    // the sample direction equals the hemisphere's orientation 
    float3 normal = normalize(input.oPosition);

    float3 irradiance = float3(0.0f, 0.0f, 0.0f);

    float3 up = float3(0.0f, 1.0f, 0.0f);
    float3 right = cross(up, normal);
    up = cross(normal, right);

    float sampleDelta = 0.025f;
    float nrSamples = 0.0f;
    for (float phi = 0.0f; phi < 2.0f * PI; phi += sampleDelta)
    {
        for (float theta = 0.0f; theta < 0.5f * PI; theta += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
            float2 uv = SampleSphericalMap(sampleVec);
            irradiance += CubeMapTexture.Sample(samplerLinear, uv).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }

    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    return float4(irradiance, 1.0f);
}
*/