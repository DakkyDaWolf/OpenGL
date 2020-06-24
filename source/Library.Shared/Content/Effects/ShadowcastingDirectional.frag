#version 440 core

layout (binding = 0) uniform sampler2D ColorTextureSampler;
layout (binding = 1) uniform sampler2D NormalMapSampler;

uniform vec4 AmbientColor;
uniform vec4 LightColor;
uniform vec3 LightPosition;
uniform vec3 CameraPosition;
uniform vec4 SpecularColor;
uniform float SpecularPower;
uniform vec4 FogColor;
uniform float LightInnerAngle = 0.75f;
uniform float LightOuterAngle = 0.25f;

in VS_OUTPUT
{
	vec2 TextureCoordinate;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec3 LightLookDirection;
	float Attenuation;
	vec3 WorldPosition;
	float FogAmount;
} IN;

out vec4 Color;

void main()
{	
	if (IN.FogAmount == 1.0f)
	{
		Color = vec4(FogColor.rgb, 1.0f);
		return;
	}

    vec3 lightDirection = normalize(LightPosition - IN.WorldPosition);
	vec3 viewDirection = normalize(CameraPosition - IN.WorldPosition);

	vec3 sampledNormal = (2 * texture(NormalMapSampler, IN.TextureCoordinate).xyz) - 1.0f;
	vec3 normal = normalize(IN.Normal + (sampledNormal.x * IN.Tangent) + (sampledNormal.y * IN.Binormal));

	float n_dot_l = dot(lightDirection, normal);
	vec3 halfVector = normalize(lightDirection + viewDirection);
	float n_dot_h = dot(normal, halfVector);

	vec3 lightLookDirection = normalize(IN.LightLookDirection);

	vec4 sampledColor = texture(ColorTextureSampler, IN.TextureCoordinate);
	vec3 ambient = AmbientColor.rgb * sampledColor.rgb;
	vec3 diffuse = clamp(LightColor.rgb * n_dot_l * sampledColor.rgb, 0.0f, 1.0f) * IN.Attenuation;	
	
	// specular = N.H^n with gloss map stored in color texture's alpha channel
	vec3 specular = SpecularColor.rgb * min(pow(clamp(n_dot_h, 0.0f, 1.0f), SpecularPower), sampledColor.w) * IN.Attenuation;

	float spotFactor = 0.0f;
	float lightAngle = dot(lightLookDirection, lightDirection);	
	if (lightAngle > 0.0f)
	{
    	spotFactor = smoothstep(LightOuterAngle, LightInnerAngle, lightAngle);
	}

	Color.rgb = ambient + (spotFactor * (diffuse + specular));
	Color.a = sampledColor.a;

	Color.rgb = mix(Color.rgb, FogColor.rgb, IN.FogAmount);
}