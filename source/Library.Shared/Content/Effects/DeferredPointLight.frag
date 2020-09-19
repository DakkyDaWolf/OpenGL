#version 440 core

layout (binding = 0) uniform sampler2D Position;
layout (binding = 1) uniform sampler2D Normal;
layout (binding = 2) uniform sampler2D AlbedoSpec;

uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform float LightRadius = 10.0f;
uniform vec2 ScreenDimensions;

uniform vec3 CameraPosition;

uniform float SpecularPower = 10.f;

out vec4 Color;

void main()
{
	vec2 texCoords = gl_FragCoord.xy / ScreenDimensions;
    vec3 FragPos = texture(Position, texCoords).rgb;
    vec3 normal = normalize(texture(Normal, texCoords).rgb);
	vec4 albSpec = texture(AlbedoSpec, texCoords);
    vec3 Albedo = albSpec.rgb;
    float SpecularClamp = albSpec.a;

	//calculation variables
    vec3 lightDirection = normalize(LightPosition - FragPos);
	vec3 viewDirection = normalize(CameraPosition - FragPos);
	float n_dot_l = dot(normal, lightDirection);
	vec3 halfVector = normalize(lightDirection + viewDirection);
	float n_dot_h = dot(normal, halfVector);
	
	//coefficient for light falloff as it becomes distant
	float attenuation = clamp(1.0f - (length(LightPosition - FragPos) / LightRadius), 0.0f, 1.0f);
	
	//calculating diffuse lighting
	vec3 diffuse = LightColor.rgb * clamp(n_dot_l, 0.0f, 1.0f) * Albedo;

	//calculate specular lighting
	vec3 specular = LightColor.rgb * min(pow(clamp(n_dot_h, 0.0f, 1.0f), SpecularPower), SpecularClamp);

	Color.rgb = (specular + diffuse) * attenuation;
	Color.a = 1;

} 