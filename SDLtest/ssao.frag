#version 330 core

out float ssaoLevel;

in vec2 texCoord0;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D ssaoNoise;

uniform vec3 ssaoKernel[16];
uniform int screenWidth;
uniform int screenHeight;

uniform mat4 projection;

int kernelSize = 16;
float radius = 3;

vec2 noiseScale = vec2(screenWidth/4.0f, screenHeight/4.0f); 

void main()
{
    vec4 fragPos = texture(gPosition, texCoord0);
    vec3 normal = texture(gNormal, texCoord0).rgb;
    vec3 randomVec = texture(ssaoNoise, texCoord0 * noiseScale).xyz;

    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    // Calculate occlusion factor
    float occlusion = 0.0;

    for(int i = 0; i < kernelSize; ++i)
    {
        vec3 sample = TBN * ssaoKernel[i];
        sample = fragPos.xyz + sample * radius; 
        
        vec4 offset = projection * vec4(sample, 1.0);
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;
        
        float sampleDepth = texture(gPosition, offset.xy).a;
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / 8 / abs(fragPos.a - sampleDepth ));
        occlusion += (sampleDepth < -sample.z && sampleDepth != 0 ? 1.0 : 0.0) * rangeCheck;
    }
    occlusion = 1.0 - (occlusion / kernelSize);


    ssaoLevel = pow(occlusion, 1.5);
}