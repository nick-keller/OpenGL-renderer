#version 400 core

in vec3 in_Vertex;
in vec2 in_TexCoord;
in vec3 in_Normal;
in vec3 in_Tangent;

out vec3 position0;
out vec3 normal0;
out vec3 tangent0;
out vec2 texCoord0;
out mat3 TBN;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	vec4 viewPosition = view * model * vec4(in_Vertex, 1);

	gl_Position = projection * viewPosition;

	position0 = viewPosition.xyz;
	texCoord0 = in_TexCoord;
	
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
	normal0 = normalMatrix * in_Normal;
	tangent0 = normalMatrix * in_Tangent;
	
	vec3 N = normalize(normal0);
	vec3 T = normalize(tangent0);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(T, N);
    TBN = mat3(T, B, N);
}