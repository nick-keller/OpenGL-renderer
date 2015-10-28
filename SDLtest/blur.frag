#version 330 core

in vec2 texCoord0;

out float blured;

uniform sampler2D ssaoLevel;

void main()
{ 
    vec2 texelSize = 1.0 / vec2(textureSize(ssaoLevel, 0));

    float result = 0.0;

    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ssaoLevel, texCoord0 + offset).r;
        }
    }

    blured = result / (4.0 * 4.0);
}