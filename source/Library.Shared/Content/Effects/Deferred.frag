#version 440 core

layout (binding = 0) uniform sampler2D ColorTextureSampler;

layout (location = 0) out vec3 Position;
layout (location = 1) out vec3 Normal;
layout (location = 2) out vec4 AlbedoSpec;

uniform float SpecularPower;

in VS_OUTPUT
{
	vec2 TextureCoordinate;
	vec3 Normal;
	vec3 WorldPosition;
} IN;

void main()
{
	vec3 sampledColor = texture(ColorTextureSampler, IN.TextureCoordinate).rgb;
	
	Position = IN.WorldPosition;
	Normal = normalize(IN.Normal);
	AlbedoSpec.rgb = sampledColor.rgb;
	AlbedoSpec.a = SpecularPower;
}