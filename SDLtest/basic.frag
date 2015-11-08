#version 330 core

in vec2 texCoord0;

out vec4 color;

uniform sampler2D textureDiffuse;
uniform sampler2D bloom;
uniform sampler2D gPosition;

void main()
{ 
    vec3 hdrColor = texture(textureDiffuse, texCoord0).rgb + texture(bloom, texCoord0).rgb;
    vec3 result = vec3(1.0) - exp(-hdrColor * 1.5);
	float vignetage = 1 - smoothstep(1, 1.7, length(texCoord0 * 2 - 1));
    color = mix(vec4(result, 1.0f), vec4(1,1, 1,1), clamp((texture(gPosition, texCoord0).a -5) / 50, 0, 1)) * vignetage;
}