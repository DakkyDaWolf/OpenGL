#version 440 core

layout (binding = 0) uniform sampler2D Position;
layout (binding = 1) uniform sampler2D Normal;
layout (binding = 2) uniform sampler2D AlbedoSpec;

uniform vec4 AmbientColor;

uniform vec3 LightPosition;
uniform vec4 LightColor;
uniform float LightFalloffRange = 10.0f;
uniform vec4 LightLookDirection;
uniform float LightInnerAngle;
uniform float LightOuterAngle;

uniform vec3 CameraPosition;

uniform vec4 FogColor;
uniform float FogStart = 500.0f;
uniform float FogRange = 1000.0f;

uniform float SpecularPower = 10.f;

in VS_OUTPUT
{
	vec2 TexCoords;
} IN;

out vec4 Color;

void main()
{
    vec3 FragPos = texture(Position, IN.TexCoords).rgb;
    vec3 normal = normalize(texture(Normal, IN.TexCoords).rgb);
	vec4 albSpec = texture(AlbedoSpec, IN.TexCoords);
    vec3 Albedo = albSpec.rgb;
    float SpecularClamp = albSpec.a;
	

	float fogAmount = clamp((distance(FragPos, CameraPosition) - FogStart) / FogRange, 0.0f, 1.0f);
    
    if (fogAmount == 1.0f)
	{
		Color = vec4(FogColor.rgb, 1.0f);
		return;
	}
	
	//base ambient color
	vec3 ambient = AmbientColor.rgb * Albedo;

	//calculation variables
    vec3 lightDirection = normalize(LightPosition - FragPos);
	vec3 viewDirection = normalize(CameraPosition - FragPos);
	float n_dot_l = dot(normal, lightDirection);
	vec3 halfVector = normalize(lightDirection + viewDirection);
	float n_dot_h = dot(normal, halfVector);
	
	//coefficient for light falloff as it becomes distant
	float attenuation = clamp(1.0f - (length(LightPosition - FragPos) / LightFalloffRange), 0.0f, 1.0f);

	//spotlight calculations
	float spotFactor = 0.0f;
	vec3 lightLookDirection = normalize(LightLookDirection.xyz);
	float lightAngle = dot(lightLookDirection, lightDirection);

	if (lightAngle > 0.0f)
	{
    	spotFactor = clamp(smoothstep(LightOuterAngle, LightInnerAngle, lightAngle), 0.0f, 1.0f);
	}
	
	//calculating diffuse lighting
	vec3 diffuse = LightColor.rgb * clamp(n_dot_l, 0.0f, 1.0f) * attenuation * Albedo;

	//calculate specular lighting
	vec3 specular = LightColor.rgb * min(pow(clamp(n_dot_h, 0.0f, 1.0f), SpecularPower), SpecularClamp) * attenuation;

	//Color.rgb += Albedo * (spotFactor * (diffuse + specular));

	Color.rgb = ambient + ((specular + diffuse) * spotFactor);
	Color.a = 1;

	Color.rgb = mix(Color.rgb, FogColor.rgb, fogAmount);

} 