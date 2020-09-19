#version 440 core

layout (binding = 0) uniform sampler2D Position;
layout (binding = 1) uniform sampler2D Normal;
layout (binding = 2) uniform sampler2D AlbedoSpec;

uniform vec3 LightColor;

in VS_OUTPUT
{
	vec2 TexCoords;
} IN;

out vec4 Color;

void main()
{
    Color = vec4(texture(AlbedoSpec, IN.TexCoords).rgb * LightColor,1);
} 