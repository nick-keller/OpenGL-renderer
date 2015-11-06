#version 400 core

out vec4 color;

in vec3 position0;
in vec3 normal0;
in vec2 texCoord0;

uniform sampler2D textureDiffuse;

void main() {
	color = texture(textureDiffuse, texCoord0);
}