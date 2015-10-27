#version 330 core

in vec2 texCoord0;

out vec4 color;

uniform sampler2D textureDiffuse;

void main()
{ 
    color = texture(textureDiffuse, texCoord0);
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    color = vec4(average, average, average, 1.0);
}