#version 400 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

in vec3 position0;
in vec3 normal0;
in vec3 tangent0;
in vec2 texCoord0;
in vec3 eyePos0;
in mat3 TBN;

uniform sampler2D textureDiffuse;
uniform sampler2D textureNormal;
uniform sampler2D textureBump;

vec3 computeNormal(vec2 texCoords);
vec2 ParallaxMapping(vec3 toEye);

void main() {
	vec3 toEye = normalize(eyePos0 - position0);
	vec2 texCoords = ParallaxMapping(toEye);
	vec3 normal = computeNormal(texCoords);
	
	gPosition = position0;
	gNormal = normal;
    gAlbedo = texture(textureDiffuse, texCoords).rgb;
}

vec2 ParallaxMapping(vec3 toEye)
{ 
	toEye = inverse(TBN) * toEye;
	
    float numLayers = mix(30, 3, abs(dot(vec3(0, 0, 1), toEye)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0;
    vec2 P = toEye.xy / 32; 
    vec2 deltaTexCoords = P / numLayers;
	
	vec2  currentTexCoords     = texCoord0;
	float currentDepthMapValue = texture(textureBump, currentTexCoords).r;
  
	while(currentLayerDepth < currentDepthMapValue)
	{
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