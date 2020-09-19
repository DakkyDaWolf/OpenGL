#version 440 core

layout (binding = 0) uniform sampler2D ColorTextureSampler;

in VS_OUTPUT
{
	vec2 TexCoords;
} IN;

out vec4 Color;

void main()
{
    Color = vec4(texture(ColorTextureSampler, IN.TexCoords).rgb,1);
} 