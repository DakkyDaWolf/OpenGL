#version 440 core

layout (binding = 0) uniform sampler2D Position;
layout (binding = 1) uniform sampler2D Normal;
layout (binding = 2) uniform sampler2D AlbedoSpec;

uniform vec4 AmbientColor;

uniform vec3 LightPosition;
uniform vec4 LightColor;
uniform float LightFalloffRange = 10.0f;
uniform vec3 LightLookDirection;
uniform float LightInnerAngle;
uniform float LightOuterAngle;

uniform vec3 CameraPosition;

uniform vec4 FogColor;
uniform float FogStart = 30.0f;
uniform float FogRange = 60.0f;


in vec2 TexCoords;
out vec4 Color;

void main()
{
    vec3 FragPos = texture(Position, TexCoords).rgb;
    vec3 Normal = texture(Normal, TexCoords).rgb;
    vec3 Albedo = texture(AlbedoSpec, TexCoords).rgb;
    float Specular = texture(AlbedoSpec, TexCoords).a;

	float fogAmount = clamp((distance(FragPos, CameraPosition) - FogStart) / FogRange, 0.0f, 1.0f);
    
    if (fogAmount == 1.0f)
	{
		Color = vec4(FogColor.rgb, 1.0f);
		return;
	}

	float attenuation = clamp(1.0f - (length(LightPosition - FragPos) / LightFalloffRange), 0.0f, 1.0f);

    vec3 lightDirection = normalize(LightPosition - FragPos);
	vec3 viewDirection = normalize(CameraPosition - FragPos);

	vec3 normal = normalize(Normal);

	float n_dot_l = dot(lightDirection, normal);
	vec3 halfVector = normalize(lightDirection + viewDirection);
	float n_dot_h = dot(normal, halfVector);

	vec3 lightLookDirection = normalize(-LightLookDirection);

	vec3 ambient = AmbientColor.rgb * Albedo;
	vec3 diffuse = clamp(LightColor.rgb * n_dot_l * Albedo, 0.0f, 1.0f) * attenuation;	
	
	// specular = N.H^n with gloss map stored in color texture's alpha channel
	vec3 specular = vec3(1) * Specular * attenuation;

	float spotFactor = 0.0f;
	float lightAngle = dot(lightLookDirection, lightDirection);	
	if (lightAngle > 0.0f)
	{
    	spotFactor = smoothstep(LightOuterAngle, LightInnerAngle, lightAngle);
	}

	Color.rgb = ambient + (spotFactor * (diffuse + specular));
	Color.a = 1;

	Color.rgb = mix(Color.rgb, FogColor.rgb, fogAmount);

} 