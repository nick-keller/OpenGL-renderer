#version 400 core

out vec4 gPosition;
out vec3 gNormal;
out vec3 gAlbedo;

in vec3 position0;
in vec3 normal0;
in vec3 tangent0;
in vec2 texCoord0;
in mat3 TBN;

uniform sampler2D textureDiffuse;
uniform sampler2D textureNormal;
uniform sampler2D textureBump;

float LinearizeDepth(float depth) ;
vec3 computeNormal(vec2 texCoords);
vec2 ParallaxMapping(vec3 toEye);

void main() {
	float depth = LinearizeDepth(gl_FragCoord.z);
	vec3 toEye = normalize(vec3(0, 0, 0) - position0);
	vec2 texCoords = depth > 10 ? texCoord0 : ParallaxMapping(toEye);
	vec3 normal = computeNormal(texCoords);
	
	gPosition.xyz = position0;
	gPosition.a = depth;
	gNormal = normal;
    gAlbedo = texture(textureDiffuse, texCoords).rgb;
}

float LinearizeDepth(float depth) 
{
	float near = .1; 
	float far  = 100.0; 
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

vec2 ParallaxMapping(vec3 toEye)
{ 
	toEye = inverse(TBN) * toEye;
	
    float numLayers = mix(30, 8, abs(dot(vec3(0, 0, 1), toEye)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0;
    vec2 P = toEye.xy / 32; 
    vec2 deltaTexCoords = P / numLayers;
	
	vec2  currentTexCoords     = texCoord0;
	float currentDepthMapValue = texture(textureBump, currentTexCoords).r;
	int a = 0;
  
	while(currentLayerDepth < currentDepthMapValue && a < 30)
	{
		a++;
		currentTexCoords -= deltaTexCoords;
		currentDepthMapValue = texture(textureBump, currentTexCoords).r;
		currentLayerDepth += layerDepth;  
	}

	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(textureBump, prevTexCoords).r - currentLayerDepth + layerDepth;
 
	float weight = afterDepth / (afterDepth - beforeDepth);
	return prevTexCoords * weight + currentTexCoords * (1.0 - weight);
} 

vec3 computeNormal(vec2 texCoords) {
	vec3 normal = texture(textureNormal, texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);   
	return normalize(TBN * normal);
}