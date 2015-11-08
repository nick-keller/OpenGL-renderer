#version 400 core

in vec3 in_Vertex;
in vec3 in_Normal;

out vec4 position0;
out vec2 texCoord0;
out float height;

uniform mat4 projection;
uniform mat4 view;
uniform float moveFactor;
uniform vec2 offset;

void main()
{
	vec4 screenPosition = projection * view * vec4(in_Vertex + vec3(offset, 0), 1);
    gl_Position = screenPosition;
	position0 = screenPosition;
	texCoord0 = vec2(in_Vertex.x + in_Vertex.y + offset.x * abs(in_Normal.y) * .5 + offset.y * abs(in_Normal.x) * .5, -in_Vertex.z /2 - moveFactor);
	height = in_Vertex.z;
} 