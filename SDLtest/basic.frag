#version 330 core

in vec2 texCoord0;

out vec4 color;

uniform sampler2D textureDiffuse;

void main()
{ 
    color = texture(textureDiffuse, texCoord0);
}