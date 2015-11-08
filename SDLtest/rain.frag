#version 330 core

in vec4 position0;
in vec2 texCoord0;
in float height;

out vec4 color;

uniform sampler2D gPosition;
uniform sampler2D textureDiffuse;

float LinearizeDepth(float depth) ;

void main()
{    
	float distance = LinearizeDepth(gl_FragCoord.z);
	vec2 screenCoord = position0.xy / position0.w /2 + 0.5;
	float pxDepth = texture(gPosition, screenCoord).a;
	float depth = pxDepth == 0 ? 1 : pxDepth - distance;
	depth = clamp(depth, 0, 1);
	float transparency = 1 - smoothstep(4, 6, height);

	float rainValue = texture(textureDiffuse, texCoord0).r;
    color = vec4(1, 1, 1, depth * rainValue * transparency * .2);
}

float LinearizeDepth(float depth) 
{
	float near = .1; 
	float far  = 100.0; 
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}