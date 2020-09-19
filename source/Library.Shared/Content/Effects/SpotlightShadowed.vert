#version 440 core

uniform mat4 WorldViewProjection;
uniform mat4 World;
uniform vec3 LightPosition;
uniform float LightFalloffRange = 10.0f;
uniform vec3 LightLookDirection;
uniform vec3 CameraPosition;
uniform float FogStart = 30.0f;
uniform float FogRange = 60.0f;
uniform mat4 LightWVP;

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureCoordinate;
layout (location = 2) in vec3 Normal;

out VS_OUTPUT
{
	vec2 TextureCoordinate;
	vec3 Normal;
	vec3 WorldPosition;
	float Attenuation;

	vec4 LightspacePosition;
	vec3 LightLookDirection;

	float FogAmount;
} OUT;

void main()
{	
	gl_Position = WorldViewProjection * Position;
	OUT.TextureCoordinate = TextureCoordinate;
	OUT.Normal = (World * vec4(Normal, 0.0f)).xyz;
	OUT.WorldPosition = (World * Position).xyz;
	OUT.LightspacePosition = LightWVP * vec4(OUT.WorldPosition, 1);

	OUT.FogAmount = clamp((distance(OUT.WorldPosition, CameraPosition) - FogStart) / FogRange, 0.0f, 1.0f);

	vec3 lightDirection = LightPosition - OUT.WorldPosition;
	OUT.Attenuation = clamp(1.0f - (length(lightDirection) / LightFalloffRange), 0.0f, 1.0f);
	OUT.LightLookDirection = -LightLookDirection;
}