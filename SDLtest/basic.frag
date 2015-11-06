#version 330 core

in vec2 texCoord0;

out vec4 color;

uniform sampler2D textureDiffuse;
uniform sampler2D bloom;

void main()
{ 
    vec3 hdrColor = texture(textureDiffuse, texCoord0).rgb + texture(bloom, texCoord0).rgb;
    vec3 result = vec3(1.0) - exp(-hdrColor * 1.5);
    color = vec4(result, 1.0f);
}