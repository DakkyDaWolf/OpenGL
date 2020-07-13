#version 440 core

layout (binding = 0) uniform sampler2D ColorTextureSampler;
layout (binding = 1) uniform sampler2D DepthTextureSampler;

uniform vec4 AmbientColor;
uniform vec4 LightColor;
uniform vec3 LightPosition;
uniform vec3 CameraPosition;
uniform vec4 SpecularColor;
uniform float SpecularPower;

uniform vec4 FogColor;

uniform float LightInnerAngle = 0.75f;
uniform float LightOuterAngle = 0.25f;
uniform float ShadowDepthTolerance = 0.01f;
uniform uint PCFEnable = 0;

uniform vec4 Albedo;

in VS_OUTPUT
{
	vec2 TextureCoordinate;
	vec3 Normal;
	vec3 WorldPosition;
	float Attenuation;

	vec4 LightspacePosition;
	vec3 LightLookDirection;
	
	float FogAmount;
} IN;

float CalculateShadowed(vec3 scaledFragmentPosition)
{
	float mappedDepth = texture(DepthTextureSampler, scaledFragmentPosition.xy).r;
	float actualDepth = scaledFragmentPosition.z;

	if(actualDepth >= 1.f || actualDepth <= 0.f) return 1.f;

	float bias = max(ShadowDepthTolerance * (1.f - dot(IN.Normal, IN.LightLookDirection)), 0.0005f); 
	float shadow = (actualDepth - bias) > mappedDepth  ? 0.f : 1.f; 

	return shadow;
}

float CalculatePCFShadow(vec3 scaledFragmentPosition)
{
	float shadow = 0.f;
	vec2 texelSize = 1.0 / textureSize(DepthTextureSampler, 0);

	//center
	shadow += CalculateShadowed(scaledFragmentPosition);
	//upper right
	shadow += CalculateShadowed(vec3(scaledFragmentPosition.xy + texelSize, scaledFragmentPosition.z));
	//lower left
	shadow += CalculateShadowed(vec3(scaledFragmentPosition.xy - texelSize, scaledFragmentPosition.z));
	//lower right
	shadow += CalculateShadowed(vec3(scaledFragmentPosition.x + texelSize.x, scaledFragmentPosition.y - texelSize.y, scaledFragmentPosition.z));
	//upper left
	shadow += CalculateShadowed(vec3(scaledFragmentPosition.x - texelSize.x, scaledFragmentPosition.y + texelSize.y, scaledFragmentPosition.z));
	//center right
	shadow += CalculateShadowed(vec3(scaledFragmentPosition.x + texelSize.x, scaledFragmentPosition.y, scaledFragmentPosition.z));
	//center left
	shadow += CalculateShadowed(vec3(scaledFragmentPosition.x - texelSize.x, scaledFragmentPosition.y, scaledFragmentPosition.z));
	//top center
	shadow += CalculateShadowed(vec3(scaledFragmentPosition.x, scaledFragmentPosition.y + texelSize.y, scaledFragmentPosition.z));
	//bottom center
	shadow += CalculateShadowed(vec3(scaledFragmentPosition.x, scaledFragmentPosition.y - texelSize.y, scaledFragmentPosition.z));

    return shadow / 9.f;
}

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

	vec3 normal = normalize(IN.Normal);

	float n_dot_l = dot(lightDirection, normal);
	vec3 halfVector = normalize(lightDirection + viewDirection);
	float n_dot_h = dot(normal, halfVector);

	vec3 lightLookDirection = normalize(IN.LightLookDirection);

	vec4 sampledColor = vec4(mix(texture(ColorTextureSampler, IN.TextureCoordinate).rgb, Albedo.rgb, Albedo.a), 1);
	vec3 ambient = AmbientColor.rgb * sampledColor.rgb;
	vec3 diffuse = clamp(LightColor.rgb * n_dot_l * sampledColor.rgb, 0.0f, 1.0f) * IN.Attenuation;	
	
	// specular = N.H^n with gloss map stored in color texture's alpha channel
	vec3 specular = SpecularColor.rgb * min(pow(clamp(n_dot_h, 0.0f, 1.0f), SpecularPower), sampledColor.w) * IN.Attenuation;

	float spotFactor = 0.0f;
	float lightAngle = dot(lightLookDirection, lightDirection);	
	if (lightAngle > 0.0f)
	{
    	spotFactor = smoothstep(LightOuterAngle, LightInnerAngle, lightAngle);

		vec3 scaledPosition = IN.LightspacePosition.xyz / IN.LightspacePosition.w;
		scaledPosition *= 0.5f;
		scaledPosition += 0.5f;

		spotFactor *= PCFEnable * CalculatePCFShadow(scaledPosition) + (1 - PCFEnable) * CalculateShadowed(scaledPosition);
	}

	Color.rgb = ambient + (spotFactor * (diffuse + specular));
	Color.a = sampledColor.a;

	Color.rgb = mix(Color.rgb, FogColor.rgb, IN.FogAmount);
}