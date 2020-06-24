#version 440 core

uniform mat4 WorldViewProjection;
uniform mat4 World;
uniform vec3 LightLookDirection;
uniform vec3 LightPosition;
uniform vec3 CameraPosition;
uniform float FogStart = 20.0f;
uniform float FogRange = 40.0f;
uniform float LightFalloffRange = 10.0f;

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureCoordinate;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Binormal;

out VS_OUTPUT
{
	vec2 TextureCoordinate;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec3 LightLookDirection;
	float Attenuation;
	vec3 WorldPosition;
	float FogAmount;
} OUT;

void main()
{	
	gl_Position = WorldViewProjection * Position;
	OUT.TextureCoordinate = TextureCoordinate;
	OUT.Normal = (World * vec4(Normal, 0.0f)).xyz;
	OUT.Tangent = (World * vec4(Tangent, 0.0f)).xyz;
	OUT.Binormal = (World * vec4(Binormal, 0.0f)).xyz;
	OUT.WorldPosition = (World * Position).xyz;
	OUT.FogAmount = clamp((distance(OUT.WorldPosition, CameraPosition) - FogStart) / FogRange, 0.0f, 1.0f);

	vec3 lightDirection = LightPosition - OUT.WorldPosition;
	OUT.Attenuation = clamp(1.0f - (length(lightDirection) / LightFalloffRange), 0.0f, 1.0f);
	OUT.LightLookDirection = -LightLookDirection;
}