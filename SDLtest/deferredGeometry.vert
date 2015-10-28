#version 400 core

in vec3 in_Vertex;
in vec2 in_TexCoord;
in vec3 in_Normal;
in vec3 in_Tangent;

out vec3 position0;
out vec3 normal0;
out vec3 tangent0;
out vec2 texCoord0;
out vec3 eyePos0;
out mat3 TBN;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	vec4 worldPosition = model * vec4(in_Vertex, 1);

	gl_Position = projection * view * worldPosition;

	eyePos0 = (inverse(view) * vec4(0, 0, 0, 1)).xyz;
	position0 = worldPosition.xyz;
	normal0 = normalize(vec3(model * vec4(in_Normal, 0.0)));;
	tangent0 = normalize(vec3(model * vec4(in_Tangent, 0.0)));;
	texCoord0 = in_TexCoord;
	
	vec3 N = normalize(normal0);
	vec3 T = normalize(tangent0);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(T, N);
    TBN = mat3(T, B, N);
}